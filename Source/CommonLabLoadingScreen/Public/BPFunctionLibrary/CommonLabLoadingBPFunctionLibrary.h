// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonLabLoadingBPFunctionLibrary.generated.h"

class UCommonLabLoadingScreenManager;
/**
 * 
 */
UCLASS()
class COMMONLABLOADINGSCREEN_API UCommonLabLoadingBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, DisplayName="Fade", Category="Common Lab Loading", meta=(WorldContext="Object"))
	static void OnFade(UObject* Object, bool bFadeOut, float Duration, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));
	
	UFUNCTION(BlueprintCallable, DisplayName="Open Level", Category="Common Lab Loading", meta=(WorldContext="Object"))
	static void OnOpenLevel(UObject* Object, FName LoadLevel, float Duration, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));
	
	UFUNCTION(BlueprintCallable, DisplayName="Travel", Category="Common Lab Loading", meta=(WorldContext="Object"))
	static void OnTravel(UObject* Object, FName LoadLevel, float Duration, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));

	UFUNCTION(BlueprintCallable, DisplayName="Travel By SubClass", Category="Common Lab Loading", meta=(WorldContext="Object"))
	static void OnTravelBySubClass(UObject* Object, FName LoadLevel, float Duration, TSubclassOf<class UUserWidget> LoadingSubClass, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));

private:
	static UCommonLabLoadingScreenManager* GetLoadingScreenManager(UObject* WorldContextObject);
};
