// Fill out your copyright notice in the Description page of Project Settings.


#include "Activatable/CommonLabActivatableWidget.h"
#include "Activatable/CommonLabActivatableStackable.h"

void UCommonLabActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	/*
	 * 이전에 활성화된 위젯이 있다면, 해당 위젯에 OnOverlap 을 호출합니다.
	 * 즉, 현재 위젯의 Stackable 의 이전 Stackable 의 마지막 위젯을 가져와서 OnOverlap 을 호출 합니다.))
	 */
	if (ActivatableStackable.IsValid())
	{
		if (UCommonLabActivatableWidget* Prev = ActivatableStackable.Get()->GetPrevActivatableWidgetInStack(this))
		{
			Prev->NativeOnOverlap(this, true);
		}	
	}
}

void UCommonLabActivatableWidget::NativeOnDeactivated()
{
	if (ActivatableStackable.IsValid())
	{
		if (UCommonLabActivatableWidget* Prev = ActivatableStackable.Get()->GetPrevActivatableWidgetInStack(this))
		{
			Prev->NativeOnOverlap(this, false);
		}	
	}

	Super::NativeOnDeactivated();
}

void UCommonLabActivatableWidget::NativeOnOverlap(UCommonLabActivatableWidget* OtherWidget, bool bIsOverlapping)
{
	BP_OnOverlap(OtherWidget, bIsOverlapping);
}

TOptional<FUIInputConfig> UCommonLabActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case ECommonLabActivatableInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode); // 입력 을 통해 넘어오는 값을 Game 과 UI 모두 전달, 
	case ECommonLabActivatableInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode); //입력 을 통해 넘어오는 값을 Game 만 전달
	case ECommonLabActivatableInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture); // 입력 을 통해 넘어오는 값을 UI 만 전달, 마우스 캡처도 하지 않음
	case ECommonLabActivatableInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}

void UCommonLabActivatableWidget::SetActivatableStackable(UCommonLabActivatableStackable* Stackable)
{
	ActivatableStackable = Stackable;
}


