// Fill out your copyright notice in the Description page of Project Settings.

#include "Process/LoadingProcess.h"
#include "CommonLabLoadingScreenSetting.h"
#include "Blueprint/UserWidget.h"


void ULoadingProcess::Clean()
{
	Super::Clean();

	if (LoadWidget.IsValid())
		LoadWidget.Reset();
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
				SetViewportLoadWidget(true);
				// StartBlockingInput();
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

			LoadProcess = Complete;
		}
	}
	return LoadProcess != ELoadProcess::Complete;
}

void ULoadingProcess::LoadStart(float Transition, TSubclassOf<UUserWidget> WidgetSubClass, FLinearColor Color)
{
	LoadProcess = Start;
	
	FadeFunc(true, Transition, Color);
}

void ULoadingProcess::LoadEnd()
{
	// 기존에 설정된, Duration 을 기반으로 합니다.
	// 기존에 설정되어 있으며, 알파값도 그대로 (일 것으로 예상되는) FadeColorTo 를 사용합니다.
	// FadeColorTo 를 사용 은 테스트 후 별도로 관리 될 수 있습니다.
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
			LoadWidget.Reset();
	}
}
