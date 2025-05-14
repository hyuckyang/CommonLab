// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonLabLoadingScreenManager.generated.h"

UCLASS()
class UFadeProcess : public UObject
{
	GENERATED_BODY()

protected:
	enum EFadeProcess
	{
		None,
		Start,
		Tick,
		Complete,
	};

	bool bIsFade = false;
	float FadeFrom = 0.f;
	float FadeTo = 0.f;
	FLinearColor FadeColor = FLinearColor::Black;
	EFadeProcess FadeProcess = None;

	float Elapsed = 0.f;
	float Duration = 0.f;
	

	TSharedPtr<SWidget> SFadeWidget;

public:
	virtual void Clean();
	// 
	virtual void FadeFunc(bool bFadeOut, float Transition, FLinearColor Color = FLinearColor(0.f, 0.f, 0.f, 1.f));
	virtual bool FadeTick(float DeltaTime);

protected:

	void SetViewportFadeWidget(bool bIsShow);
	/*
	 * 기본으로 SCommonLabLoadingFadeSWidget 생성합니다.
	 * 다만 WidgetClass 시 이를 대체할 수 있습니다.
	 */
	void SetViewportWidget(bool bIsShow, TSharedPtr<SWidget>& SWidget, const TSubclassOf<UUserWidget>& WidgetClass, int32 ZOrder) const;
	
	
};

UCLASS()
class UOpenLevelProcess : public UObject
{
	GENERATED_BODY()

public:
	
};

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
	
};
