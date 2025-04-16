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

	template<typename Activatable = UCommonLabActivatableWidget>
	Activatable* AddWidgetToStackable(TSubclassOf<UCommonLabActivatableWidget> ActivatableClass, TFunctionRef<void(Activatable&)> InitFunc)
	{
		return AddWidget<Activatable>(ActivatableClass, InitFunc);
	}

	
	
	void SetPrevStackable(UCommonLabActivatableStackable* Stackable);
	
protected:

	TWeakObjectPtr<UCommonLabActivatableStackable> PrevStackable;

	
	
	
	
};
