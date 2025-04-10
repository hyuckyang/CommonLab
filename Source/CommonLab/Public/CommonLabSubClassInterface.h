// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonLabSubClassInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCommonLabSubClassInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONLAB_API ICommonLabSubClassInterface
{
	GENERATED_BODY()
	
public:

	// 최초 및, 레벨 전환 시 호출
	// GameInstance :: AddLocalPlayer () => CommonLabSubsystem :: NotifyPlayerAdded () 호출
	// LocalPlayer :: OnPlayerControllerSet (Delegate / PlayerController :: ReceivePlayer () )  호출
	virtual void Initialize() = 0;

	// 레벨 전환, 그리고 로컬플레이어 파괴시 호출
	// GameInstance :: RemoveLocalPlayer () => CommonLabSubsystem :: NotifyPlayerDestroyed () 호출	
	// LocalPlayer :: OnPlayerControllerSet (Delegate / PlayerController :: ReceivePlayer () )  호출
	virtual void Released() = 0;

	// 완전 파괴 시 호출 ( Destroyed 후, CommonLabSubsystem 의 Deinitialize 에서 파괴 됩니다 )
	// GameInstance :: RemoveLocalPlayer () => CommonLabSubsystem :: NotifyPlayerDestroyed () 호출
	virtual void Destroyed() = 0;

};
