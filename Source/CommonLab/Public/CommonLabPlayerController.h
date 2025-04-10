// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CommonLabPlayerController.generated.h"

class APawn;
class UObject;

/**
 * 
 */
UCLASS()
class COMMONLAB_API ACommonLabPlayerController : public APlayerController
{
	GENERATED_BODY()

	// 로컬플레이가 할당 
	virtual void ReceivedPlayer() override;

	// 플레이어 캐릭터가 할당/빙의/해체 등
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
protected:
	// 서버에서 플레이어 스테이트를 복제 시 호출 됩니다.
	virtual void OnRep_PlayerState() override;
};
