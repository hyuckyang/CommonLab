// Fill out your copyright notice in the Description page of Project Settings.

#include "BPFunctionLibrary/CommonLabLoadingBPFunctionLibrary.h"

#include "CommonLabLoadingScreenManager.h"

void UCommonLabLoadingBPFunctionLibrary::OnFade(UObject* Object, bool bFadeOut, float Duration, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnFade(bFadeOut, Duration, Color);
	}
}

void UCommonLabLoadingBPFunctionLibrary::OnLoadLevel(UObject* Object, FName LoadLevel, float Duration, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnLoadLevel(LoadLevel, Duration, Color);
	}
}

void UCommonLabLoadingBPFunctionLibrary::OnLoadLevelBySubClass(UObject* Object, FName LoadLevel, float Duration, TSubclassOf<class UUserWidget> LoadingSubClass, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnLoadLevelBySubClass(LoadLevel, Duration, LoadingSubClass, Color);
	}
}

UCommonLabLoadingScreenManager* UCommonLabLoadingBPFunctionLibrary::GetLoadingScreenManager(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UCommonLabLoadingScreenManager>();
}
