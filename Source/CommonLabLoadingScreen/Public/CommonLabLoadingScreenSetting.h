// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CommonLabLoadingScreenSetting.generated.h"

/**
 * 
 */
UCLASS(Config=GAME, defaultconfig, meta=(DisplayName="Common Lab Loading Screen Setting"))
class COMMONLABLOADINGSCREEN_API UCommonLabLoadingScreenSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCommonLabLoadingScreenSetting();

	UPROPERTY(Config, EditAnywhere, Category = CommonLabLoadingScreen)
	int32 ZOrder = 10000;

	UPROPERTY(config, EditAnywhere, Category = CommonLabLoadingScreen, meta=(BlueprintBaseOnly=true, MetaClass="/Script/UMG.UserWidget"))
	FSoftClassPath DefaultWidgetClass;
};
