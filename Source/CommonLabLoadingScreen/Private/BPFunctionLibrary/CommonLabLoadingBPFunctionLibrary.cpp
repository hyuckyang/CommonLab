// Fill out your copyright notice in the Description page of Project Settings.

#include "BPFunctionLibrary/CommonLabLoadingBPFunctionLibrary.h"

#include "CommonLabLoadingScreenManager.h"
#include "Kismet/GameplayStatics.h"

void UCommonLabLoadingBPFunctionLibrary::OnFade(UObject* Object, bool bFadeOut, float Duration, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnFade(bFadeOut, Duration, Color);
	}
}

void UCommonLabLoadingBPFunctionLibrary::OnOpenLevel(UObject* Object, FName LoadLevel, float Duration, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnLoadScreen(TDelegate<bool()>::CreateLambda([Object, LoadLevel]()
		{
			UGameplayStatics::OpenLevel(Object, LoadLevel);
			return true;
		}),
		Duration, Color);
	}
}

void UCommonLabLoadingBPFunctionLibrary::OnTravel(UObject* Object, FName LoadLevel, float Duration, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnLoadScreen(TDelegate<bool()>::CreateLambda([LoadLevel]()
		{
			// Travel ..
			return true;
		}),
		Duration, Color);
	}
}

void UCommonLabLoadingBPFunctionLibrary::OnTravelBySubClass(UObject* Object, FName LoadLevel, float Duration, TSubclassOf<class UUserWidget> LoadingSubClass, FLinearColor Color)
{
	if (UCommonLabLoadingScreenManager* LoadingScreenManager = GetLoadingScreenManager(Object))
	{
		LoadingScreenManager->OnLoadScreenBySubClass(TDelegate<bool()>::CreateLambda([LoadLevel]()
		{
			// 
			return true;
		}),
			Duration, LoadingSubClass, Color);
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
