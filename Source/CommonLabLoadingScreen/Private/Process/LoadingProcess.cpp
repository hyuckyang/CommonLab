// Fill out your copyright notice in the Description page of Project Settings.

#include "Process/LoadingProcess.h"
#include "Blueprint/UserWidget.h"
#include "CommonLabLoadingScreenSetting.h"
#include "CommonLabLoadingWidgetInterface.h"
#include "Framework/Application/IInputProcessor.h"

#pragma region InputProcess , 입력값 처리 -> CommonLoadingScreen 의 FLoadingScreenInputPreProcessor 와 99% 동일 합니다.

class FLoadingProcessInputPreProcess : public IInputProcessor
{
private:
	TWeakObjectPtr<UUserWidget> InputWidget;
	
public:
	FLoadingProcessInputPreProcess(UUserWidget* Widget)
	{
		if (Widget && Widget->GetClass()->ImplementsInterface(UCommonLabLoadingWidgetInterface::StaticClass()))
		{
			InputWidget = Widget;
		}
	}
	virtual ~FLoadingProcessInputPreProcess() { }

	bool CanEatInput() const
	{
		// 에디터가 아닌 경우에만 입력을 막습니다.
		return !GIsEditor;
	}

	//~IInputProcess interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override { }
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (InputWidget.IsValid())
		{
			ICommonLabLoadingWidgetInterface::Execute_OnKeyPressedEvent(InputWidget.Get(), InKeyEvent);
		}
		
		return CanEatInput();
	}
	
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (InputWidget.IsValid())
		{
			ICommonLabLoadingWidgetInterface::Execute_OnKeyReleaseEvent(InputWidget.Get(), InKeyEvent);
		}
		
		return CanEatInput();
	}

	// 차후 필요시 
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override { return CanEatInput(); }
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override { return CanEatInput(); }
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override { return CanEatInput(); }
	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override { return CanEatInput(); }
	//~End of IInputProcess interface
};

#pragma endregion 

void ULoadingProcess::Clean()
{
	Super::Clean();

	SetViewportLoadWidget(false);
}

bool ULoadingProcess::FadeTick(float DeltaTime)
{
	if (LoadProcess == Start)
		LoadProcess = Tick;

	if (Super::FadeTick(DeltaTime) == false)
	{
		if (LoadProcess == Tick)
		{
			// Fade In 경우 바로 종료 합니다
			if (!bIsFade)
			{
				LoadProcess = Complete;
			}
			else
			{
				SetBlockingInput(true); // -> 로딩 대기 부터 입력을 막습니다.
				SetViewportLoadWidget(true);
				LoadProcess = LoadWaitFrame;
			}
		}
		else if (LoadProcess == LoadWaitFrame)
		{
			
			LoadProcess = Load;
		}
		else if (LoadProcess == Load)
		{
			// Next Level Func
			// true 일 때 Complete 를 시도 합니다.
			if (LoadCompleteDelegate.IsBound())
			{
				if (LoadCompleteDelegate.Execute())
				{
					LoadProcess = Complete;
				}
			}
		}
	}
	return LoadProcess != Complete;
}

void ULoadingProcess::LoadStart(float Transition, const TSubclassOf<UUserWidget>& WidgetSubClass, FLinearColor Color, const TDelegate<bool()>& LoadDelegate)
{
	LoadProcess = Start;
	LoadSubClass = WidgetSubClass;
	LoadCompleteDelegate = LoadDelegate;
	
	FadeFunc(true, Transition, Color);
}

void ULoadingProcess::LoadStart(float Transition, const TSubclassOf<UUserWidget>& WidgetSubClass, FLinearColor FadeFromColor, FLinearColor FadeToColor, const TDelegate<bool()>& LoadDelegate)
{
	LoadProcess = Start;
	LoadSubClass = WidgetSubClass;
	LoadCompleteDelegate = LoadDelegate;
	
	FadeFunc(true, Transition, FadeFromColor, FadeToColor);
}

void ULoadingProcess::LoadEnd()
{
	// 기존에 설정된, Duration 을 기반으로 합니다.
	// 기존에 설정되어 있으며, 알파값도 그대로 (일 것으로 예상되는) FadeColorTo 를 사용합니다.
	// FadeColorTo 를 사용 은 테스트 후 별도로 관리 될 수 있습니다.
	SetViewportLoadWidget(false);
	SetBlockingInput(false); // -> 로딩 대기 부터 막힌 입력을 다시 풀어냅니다.
	FadeFunc(false, Duration, FadeColorTo);
}

void ULoadingProcess::SetViewportLoadWidget(bool bIsShow)
{
	int32 Order = 0;
	if (const UCommonLabLoadingScreenSetting* Setting = GetDefault<UCommonLabLoadingScreenSetting>())
	{
		Order = Setting->ZOrder;
	}

	TSharedPtr<SWidget> SWidget;
	UUserWidget* Widget = SetViewportWidget(bIsShow, SWidget, LoadSubClass, Order + 2);
	if (bIsShow && Widget)
	{
		LoadWidget = Widget;	
	}
	else
	{
		if (LoadWidget.IsValid())
		{
			RemoveViewportWidget(LoadWidget->TakeWidget());
			LoadWidget.Reset();
		}
	}
}

void ULoadingProcess::SetBlockingInput(bool bIsInputBlock)
{
	if (bIsInputBlock)
	{
		if (!InputProcessor.IsValid())
		{
			InputProcessor = MakeShareable<FLoadingProcessInputPreProcess>(new FLoadingProcessInputPreProcess(LoadWidget.Get()));
			FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 0);
		}
	}
	else
	{
		if (InputProcessor.IsValid())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
			InputProcessor.Reset();
		}
	}
}
