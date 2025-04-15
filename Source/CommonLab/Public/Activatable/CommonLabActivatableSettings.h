// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "CommonLabActivatableSettings.generated.h"

USTRUCT()
struct FCommonLabActivatableStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=CommonLabActivatable)
	FGameplayTag ActivatableTag;

	UPROPERTY(EditAnywhere, Category=CommonLabActivatable, meta=(BlueprintBaseOnly=true, AllowAbstract=false, MetaClass="/Script/CommonLab.CommonLabActivatableWidget"))
	FSoftClassPath ActivatableWidget;
};

USTRUCT()
struct FCommonLabActivatableLevelStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PrimaryActivatable, meta=(AllowedClasses = "/Script/Engine.World"))
	FSoftObjectPath Level;

	UPROPERTY(EditAnywhere, Category = PrimaryActivatable)
	FCommonLabActivatableStruct ActivatableStruct;
	
};


/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DiesplayName = "Common Lab Activatable Setting"))

class COMMONLAB_API UCommonLabActivatableSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = CommonLabActivatable)
	TArray<FGameplayTag> ActivatableStackTags;
	
	UPROPERTY(Config, EditAnywhere, Category = CommonLabActivatable)
	FSoftClassPath RootLayoutClass;

	/*
	 * (LevelActivatableClasses 에 등록되지 않는) 모든 레벨이 기본으로 등록되는 기본 UI Widget 
	 */
	UPROPERTY(Config, EditAnywhere, Category = CommonLabActivatable)
	FCommonLabActivatableStruct BasedActivatable;

	/*
	 * 특정 레벨 에 출력되는 UI Widget
	 */
	UPROPERTY(Config, EditAnywhere, Category = CommonLabActivatable)
	TArray<FCommonLabActivatableLevelStruct> LevelActivatableStructs;
};
