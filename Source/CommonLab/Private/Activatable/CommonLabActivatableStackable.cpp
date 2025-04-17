// Fill out your copyright notice in the Description page of Project Settings.


#include "Activatable/CommonLabActivatableStackable.h"
#include "Activatable/CommonLabActivatableWidget.h"

void UCommonLabActivatableStackable::SetPrevStackable(UCommonLabActivatableStackable* Stackable)
{
	if (Stackable)
	{
		PrevStackable = Stackable;
	}
}

UCommonLabActivatableWidget* UCommonLabActivatableStackable::GetPrevActivatableWidgetInStack(UCommonLabActivatableWidget* CurrentWidget)
{
	UCommonLabActivatableWidget* PrevActivatable = nullptr;
	int32 CurrentIndex = WidgetList.IndexOfByKey(CurrentWidget);
	if (CurrentIndex != INDEX_NONE && CurrentIndex > 0)
	{
		PrevActivatable = Cast<UCommonLabActivatableWidget>(WidgetList[CurrentIndex - 1]);
	}
	else
	{
		UCommonLabActivatableStackable* Stackable = PrevStackable.IsValid() ? PrevStackable.Get() : nullptr;
		while (Stackable)
		{
			if (Stackable->WidgetList.Num() > 0)
			{
				int32 LastIndex = Stackable->WidgetList.Num() - 1;
				if (UCommonLabActivatableWidget* LastWidget = Cast<UCommonLabActivatableWidget>(Stackable->WidgetList[LastIndex]))
				{
					PrevActivatable = LastWidget;
					break;
				}	
			}
			Stackable = Stackable->GetPrevStackable();
		}
	}

	return PrevActivatable;
}

