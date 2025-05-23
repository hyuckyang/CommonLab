// Fill out your copyright notice in the Description page of Project Settings.

#include "Process/FadeProcess.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Engine/UserInterfaceSettings.h"
#include "CommonLabLoadingScreenSetting.h"

#pragma region Fade SWidget

void SFadeCompoundWidget::Construct(const FArguments& Args, FColor Color)
{
	TSharedRef<SOverlay> Root = SNew(SOverlay);
	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill);
	
	ChildSlot
	[
		SNew(SDPIScaler) //
		.DPIScale(this, &SFadeCompoundWidget::GetDPIScale)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(SFadeImage, SImage)
				.Visibility(EVisibility::HitTestInvisible)
				.ColorAndOpacity(Color)
			]
		]
	];
}

void SFadeCompoundWidget::SetFadeColor(const FLinearColor& Color) const
{
	if (SFadeImage.IsValid())
	{
		SFadeImage->SetColorAndOpacity(Color);
	}
}

float SFadeCompoundWidget::GetDPIScale() const
{
	const FVector2D& DrawSize = GetCachedGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size(static_cast<int32>(DrawSize.X), static_cast<int32>(DrawSize.Y));
	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size); // Project Setting -> User Interface 에 설정된 DPI Scals Curve 등 참조
}

#pragma endregion


void UFadeProcess::Clean()
{
	SetViewportFadeWidget(false);
}

void UFadeProcess::FadeFunc(bool bFadeOut, float Transition, FLinearColor FadeColor)
{
	bIsFade = bFadeOut;
	FadeColorFrom = FadeColor;
	FadeColorFrom.A = bIsFade ? 0.f : 1.f;
	FadeColorTo = FadeColor;
	FadeColorTo.A = bIsFade ? 1.f : 0.f;
	FadeProcess = Start;
	
	Elapsed = 0.f;
	Duration = Transition;

	SetViewportFadeWidget(true);
}

void UFadeProcess::FadeFunc(bool bFadeOut, float Transition, FLinearColor FadeFromColor, FLinearColor FadeToColor)
{
	bIsFade = bFadeOut;
	FadeColorFrom = FadeFromColor;
	FadeColorTo = FadeToColor;
	FadeColorTo.A = bIsFade ? 1.f : 0.f;
	FadeProcess = Start;
	
	Elapsed = 0.f;
	Duration = Transition;
	
	SetViewportFadeWidget(true);
}

bool UFadeProcess::FadeTick(float DeltaTime)
{
	if (FadeProcess == Start)
		FadeProcess = Tick;

	if (Elapsed < Duration)
	{
		const FLinearColor InterpColor = FMath::Lerp(FadeColorFrom, FadeColorTo, Elapsed / Duration);
		if (SFadeWidget.IsValid())
		{
			SFadeWidget->SetFadeColor(InterpColor);
		}
		Elapsed += DeltaTime;
	}
	else
	{
		if (FadeProcess != Complete)
		{
			if (SFadeWidget.IsValid())
				SFadeWidget->SetFadeColor(FadeColorTo);

			// Fade In 으로 화면에 복귀 된 경우이기에 바로 삭제 합니다.
			if (!bIsFade)
				SetViewportFadeWidget(false);
		}

		FadeProcess = Complete;
	}
	
	return FadeProcess != Complete;
}

void UFadeProcess::SetViewportFadeWidget(bool bIsShow)
{
	// 이미 존재하는데 한번 더 Show 하려 할때는 그대로 남겨둡니다. ( 연속성 )
	if (SFadeWidget.IsValid() && bIsShow)
		return;

	int32 Order = 0;
	if (const UCommonLabLoadingScreenSetting* Setting = GetDefault<UCommonLabLoadingScreenSetting>())
	{
		Order = Setting->ZOrder;
	}
	
	TSharedPtr<SWidget> SWidget;
	SetViewportWidget(bIsShow, SWidget, nullptr, Order );

	// SFadeWidget 설정
	if (SWidget.IsValid())
	{
		SFadeWidget = StaticCastSharedPtr<SFadeCompoundWidget>(SWidget);
	}
	else
	{
		if (SFadeWidget.IsValid())
		{
			RemoveViewportWidget(SFadeWidget);
			SFadeWidget.Reset();
		}
	}
}

UUserWidget* UFadeProcess::SetViewportWidget(bool bIsShow, TSharedPtr<SWidget>& SWidget, const TSubclassOf<UUserWidget>& WidgetClass, int32 ZOrder) const
{
	UUserWidget* Widget = nullptr;
	const UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return nullptr;

	const UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
		return nullptr;
	
	if(UGameViewportClient* ViewportClient = GameInstance->GetGameViewportClient())
	{
		if (bIsShow)
		{
			APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!Controller)
				return nullptr;
			
			if (WidgetClass.Get() != nullptr)
			{
				Widget = UUserWidget::CreateWidgetInstance(*Controller, WidgetClass, *WidgetClass->GetName());
				if (!Widget)
				{
					SWidget = SNew(SFadeCompoundWidget, FColor::Transparent);
				}
				else
				{
					SWidget = Widget->TakeWidget();	
				}
			}
			else
			{
				// Base Fade
				SWidget = SNew(SFadeCompoundWidget, FColor::Transparent);
			}

			if (SWidget.IsValid())
			{
				SWidget.Get()->SetVisibility(EVisibility::SelfHitTestInvisible);
				ViewportClient->AddViewportWidgetContent(SWidget.ToSharedRef(), ZOrder);
			}
		}
		else
		{
			// 현재는 의미가 없습니다.
			// if (SWidget.IsValid())
			// {
			// 	ViewportClient->RemoveViewportWidgetContent(SWidget.ToSharedRef());
			// }
			// SWidget.Reset();
		}
	}
	
	return Widget;
}

void UFadeProcess::RemoveViewportWidget(TSharedPtr<SWidget> SWidget) const
{
	const UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return ;

	const UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
		return ;
	
	if(UGameViewportClient* ViewportClient = GameInstance->GetGameViewportClient())
	{
		if (SWidget.IsValid())
		{
			ViewportClient->RemoveViewportWidgetContent(SWidget.ToSharedRef());
		}
	}
}

