// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonLabActivatableStackable.generated.h"

class UOverlay;
class UCommonLabActivatableWidget;
class UCommonLabActivatableStackable;

/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabActivatableStackable : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()

public:

	
	

	// ~Begin Stackable Function
	void SetPrevStackable(UCommonLabActivatableStackable* Stackable);
	UCommonLabActivatableStackable* GetPrevStackable() const { return PrevStackable.IsValid() ? PrevStackable.Get() : nullptr; }
	UCommonLabActivatableWidget* GetPrevActivatableWidgetInStack(UCommonLabActivatableWidget* CurrentWidget);
	// ~End Stackable Function

protected:

	TWeakObjectPtr<UCommonLabActivatableStackable> PrevStackable;
};
