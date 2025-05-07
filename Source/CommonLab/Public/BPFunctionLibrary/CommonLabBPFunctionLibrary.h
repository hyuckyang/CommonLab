// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonLabBPFunctionLibrary.generated.h"

class UCommonLabActivatableWidget;

/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region Common Lab Activatable

	UFUNCTION(BlueprintCallable, DisplayName="Open", Category="Common Lab|Activatable", meta = (WorldContext = "Object"))
	static UCommonLabActivatableWidget* Open_ActivatableWidget(UObject* Object, FGameplayTag Tag, TSubclassOf<UCommonLabActivatableWidget> TClass);

	UFUNCTION(BlueprintCallable, DisplayName="Open By Local Player", Category="Common Lab|Activatable")
	static UCommonLabActivatableWidget* Open_ActivatableWidgetByLocalPlayer(ULocalPlayer* LocalPlayer, FGameplayTag Tag, TSubclassOf<UCommonLabActivatableWidget> TClass);
	
#pragma endregion
	
};
