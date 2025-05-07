// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFunctionLibrary/CommonLabBPFunctionLibrary.h"
#include "Activatable/CommonLabActivatableSubClass.h"
#include "GameplayTagContainer.h"

UCommonLabActivatableWidget* UCommonLabBPFunctionLibrary::Open_ActivatableWidget(UObject* Object, FGameplayTag Tag, TSubclassOf<UCommonLabActivatableWidget> TClass)
{
	if (ULocalPlayer* LocalPlayer = Object->GetWorld()->GetFirstLocalPlayerFromController())
	{
		return Open_ActivatableWidgetByLocalPlayer(LocalPlayer, Tag, TClass);
	}
	return nullptr;
}

UCommonLabActivatableWidget* UCommonLabBPFunctionLibrary::Open_ActivatableWidgetByLocalPlayer(ULocalPlayer* LocalPlayer, FGameplayTag Tag, TSubclassOf<UCommonLabActivatableWidget> TClass)
{
	if (LocalPlayer != nullptr)
	{
		if (UCommonLabActivatableSubClass* ActivatableSubClass = LocalPlayer->GetGameInstance()->GetSubsystem<UCommonLabSubsystem>()->GetSubClass<UCommonLabActivatableSubClass>(LocalPlayer))
		{
			return ActivatableSubClass->Open<UCommonLabActivatableWidget>(Tag, TClass);	
		}
	}
	return nullptr;
}
