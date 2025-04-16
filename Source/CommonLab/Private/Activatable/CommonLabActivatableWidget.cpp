// Fill out your copyright notice in the Description page of Project Settings.


#include "Activatable/CommonLabActivatableWidget.h"

#include "Activatable/CommonLabActivatableStackable.h"


void UCommonLabActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();	
}

void UCommonLabActivatableWidget::NativeOnDeactivated()
{
	if (UCommonLabActivatableWidget* Prev = nullptr)
	{
		Prev->NativeOnOverlap(this, false);
	}

	Super::NativeOnDeactivated();
}

void UCommonLabActivatableWidget::NativeOnOverlap(UCommonLabActivatableWidget* OtherWidget, bool bIsOverlapping)
{
	
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

void UCommonLabActivatableWidget::SetActivatedStackable(UCommonLabActivatableStackable* Stackable)
{
	ActivatableStackable = Stackable;
}

