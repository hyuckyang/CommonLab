// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLabLoadingScreenManager.h"
#include "Engine/UserInterfaceSettings.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#pragma region Fade SWidget

class SCommonLabLoadingFadeSWidget : public SCompoundWidget
{
	
public:
	SLATE_BEGIN_ARGS(SCommonLabLoadingFadeSWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& Args, FColor Color)
	{
		TSharedRef<SOverlay> Root = SNew(SOverlay);
		Root->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill);

		ChildSlot
		[
			SNew(SDPIScaler) //
			.DPIScale(this, &SCommonLabLoadingFadeSWidget::GetDPIScale)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Visibility(EVisibility::HitTestInvisible)
					.ColorAndOpacity(Color)
				]
			]
		];
	}

private:
	
	float GetDPIScale() const
	{
		const FVector2D& DrawSize = GetCachedGeometry().ToPaintGeometry().GetLocalSize();
		const FIntPoint Size(static_cast<int32>(DrawSize.X), static_cast<int32>(DrawSize.Y));
		return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size); // Project Setting -> User Interface 에 설정된 DPI Scals Curve 등 참조
	}
};

#pragma endregion 

#pragma region Fade Process

void UFadeProcess::Clean()
{
	if (SFadeWidget.IsValid())
		SFadeWidget.Reset();
}

void UFadeProcess::FadeFunc(bool bFadeOut, float Transition, FLinearColor Color)
{
	bIsFade = bFadeOut;
	FadeFrom = bIsFade ? 0.f : 1.f;
	FadeTo = bIsFade ? 1.f : 0.f;

	
	FadeProcess = Start;
}

bool UFadeProcess::FadeTick(float DeltaTime)
{
	if (FadeProcess == Start)
		FadeProcess = Tick;

	return FadeProcess != Complete;
}

void UFadeProcess::SetViewportFadeWidget(bool bIsShow)
{
	// 이미 존재하는데 한번 더 Show 하려 할때는 그대로 남겨둡니다. ( 연속성 )
	if (SFadeWidget.IsValid() && bIsShow)
		return;
	
	SetViewportWidget(bIsShow, SFadeWidget, nullptr, /* ZOrder */ 0 );
	if (SFadeWidget.IsValid())
	{
		if (TSharedPtr<SCommonLabLoadingFadeSWidget> FadeWidget = StaticCastSharedPtr<SCommonLabLoadingFadeSWidget>(SFadeWidget))
		{
			FadeWidget->SetColorAndOpacity(FadeColor);
		}
	}
}

void UFadeProcess::SetViewportWidget(bool bIsShow, TSharedPtr<SWidget>& SWidget, const TSubclassOf<UUserWidget>& WidgetClass, int32 ZOrder) const
{
	const UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return;

	const UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
		return;
	
	if(UGameViewportClient* ViewportClient = GameInstance->GetGameViewportClient())
	{
		if (bIsShow)
		{
			APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!Controller)
				return;
			
			if (WidgetClass.Get() != nullptr)
			{
				UUserWidget* Widget = UUserWidget::CreateWidgetInstance(*Controller, WidgetClass, *WidgetClass->GetName());
				if (!Widget)
				{
					SWidget = SNew(SCommonLabLoadingFadeSWidget, FColor::Black);
				}
				else
				{
					SWidget = Widget->TakeWidget();	
				}
			}
			else
			{
				// Base Fade
				SWidget = SNew(SCommonLabLoadingFadeSWidget, FColor::Black);
			}

			if (SWidget.IsValid())
			{
				SWidget.Get()->SetVisibility(EVisibility::SelfHitTestInvisible);
				ViewportClient->AddViewportWidgetContent(SWidget.ToSharedRef(), ZOrder);
			}
		}
		else
		{
			if (SWidget.IsValid())
			{
				ViewportClient->RemoveViewportWidgetContent(SWidget.ToSharedRef());
			}

			SWidget.Reset();
		}
	}
}


#pragma endregion 

void UCommonLabLoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCommonLabLoadingScreenManager::Deinitialize()
{
	Super::Deinitialize();
}

bool UCommonLabLoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	// Dedicate Server 가 아닌 경우에만 생성  
	const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	const bool bIsDedicatedServer = GameInstance->IsDedicatedServerInstance();
	return !bIsDedicatedServer;
}

void UCommonLabLoadingScreenManager::Tick(float DeltaTime)
{
}

ETickableTickType UCommonLabLoadingScreenManager::GetTickableTickType() const
{
	// IsTickable 의 조건에 따라 Tick 을 활성화 합니다.
	return ETickableTickType::Conditional; 
}

bool UCommonLabLoadingScreenManager::IsTickable() const
{
	// CDO 가 아닌 생성된 객체 일 경우에만 Tick 을 활성화 합니다.
	return !HasAnyFlags(RF_ClassDefaultObject);
}

TStatId UCommonLabLoadingScreenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCommonLabLoadingScreenManager, STATGROUP_Tickables); 
}

UWorld* UCommonLabLoadingScreenManager::GetTickableGameObjectWorld() const
{

	// GameInstance 가 일관된 World 를 반환합니다.
	return GetGameInstance()->GetWorld();
}
