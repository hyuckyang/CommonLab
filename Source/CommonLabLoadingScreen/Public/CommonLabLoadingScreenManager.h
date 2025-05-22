// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLabLoadingShouldInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonLabLoadingScreenManager.generated.h"

template <typename InterfaceType> class TScriptInterface;

class UFadeProcess;
class ICommonLabLoadingShouldInterface;

/**
 * 
 */
UCLASS()
class COMMONLABLOADINGSCREEN_API UCommonLabLoadingScreenManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	// ~ USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// ~ End of USubsystem Interface

	// ~ FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;
	// ~ End of FTickableGameObject Interface

	// ~ Process Function
	void OnFade(bool bFadeOut, float Duration, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));
	void OnLoadLevel(FName LoadLevel, float Duration, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));
	void OnLoadLevelBySubClass(FName LoadLevel, float Duration, TSubclassOf<class UUserWidget> LoadingSubClass, FLinearColor Color = FLinearColor(0.f,0.f,0.f,1.f));

private:
	bool HandleProcessElapsed(float Duration, FLinearColor& OutElapsedColor, float& OutElapsedTime, bool& OutShouldConnectFade);
	// ~ End Process Function

public:
	// ~ Should Interface
	void RegisterShouldInterface(TScriptInterface<ICommonLabLoadingShouldInterface> Interface);
	void UnRegisterShouldInterface(TScriptInterface<ICommonLabLoadingShouldInterface> Interface);
	// ~ End Should Interface	
	
private:
	void HandlePreLoadMap(const FWorldContext& Context, const FString& MapName);
	void HandlePostLoadMap(UWorld* World);

	/*
	 * 로딩창이 출력되는지 여부를 체크 합니다. ( 기본부터 인터페이스 까지 )
	 * Common Loading Screen 과 99% 동일합니다
	 */
	bool IsShouldShowLoadScreen() const;
	
	void UpdateLoadScreen();
	void HideLoadScreen();

	UPROPERTY()
	TObjectPtr<UFadeProcess> Process;
	
	bool bCurrentlyInLoadMap = false; // Map 이 로드 중인지 ( Pre -> Post Load Map )
	bool bCurrentlyShowLoadScreen = false; // Load Screen 을 호출하여 Show 중인지.

	TArray<TWeakInterfacePtr<ICommonLabLoadingShouldInterface>> ShouldLoadingProcess;
};
