// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CommonLabPreLoadingScreenSetting.generated.h"

/**
 * 
 */
UCLASS(Config=GAME, defaultconfig, meta=(DisplayName="Common Lab Pre Loading Screen Setting"))
class UCommonLabPreLoadingScreenSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCommonLabPreLoadingScreenSetting();

	UPROPERTY(config, EditAnywhere, Category=CommonLabPreLoadingScreen, meta=(BlueprintBaseOnly=true, AllowedClasses = "/Script/Engine.Texture2D"))
	FSoftObjectPath PreLoadingImage;

	/*
	 * 128 x 128 의 Material 을 기준으로 제작하였습니다. 현재 테스트 중에 있습니다.
	 */
	// UPROPERTY(config, EditAnywhere, Category=Display, meta=(BlueprintBaseOnly=true, AllowedClasses = "/Script/Engine.MaterialInterface"))
	// FSoftObjectPath PreLoadingLoadIndicatorMaterial;

	// ~Begin CircleThrobber Group
	
	UPROPERTY(config, EditAnywhere, Category=CommonLabPreLoadingScreen, meta=(BlueprintBaseOnly=true, AllowedClasses = "/Script/Engine.Texture2D"))
	FSlateBrush PreLoadingCircleThrobberImage;
	
	UPROPERTY(config, EditAnywhere, Category=CommonLabPreLoadingScreen)
	int32 PreLoadingCircleNumberOfPieces = 0;
	
	UPROPERTY(config, EditAnywhere, Category=CommonLabPreLoadingScreen)
	float PreLoadingCirclePeriod = 0.f;
	
	UPROPERTY(config, EditAnywhere, Category=CommonLabPreLoadingScreen)
	float PreLoadingCircleRadius = 0.f;

	// ~End CircleThrobber Group
};
