﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFunctionLibrary/CommonLabBPFunctionLibrary.h"
#include "Activatable/CommonLabActivatableSubClass.h"
#include "GameplayTagContainer.h"
#include "Delegate/CommonLabDelegateSubClass.h"

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

void UCommonLabBPFunctionLibrary::FindExecuteCommand(UObject* Object, const FString& CommandName)
{
	if (ULocalPlayer* LocalPlayer = Object->GetWorld()->GetFirstLocalPlayerFromController())
	{
		if (UCommonLabDelegateSubClass* DelegateSubClass = LocalPlayer->GetGameInstance()->GetSubsystem<UCommonLabSubsystem>()->GetSubClass<UCommonLabDelegateSubClass>(LocalPlayer))
		{
			DelegateSubClass->FindExecute(CommandName);
		}	
	}
}
