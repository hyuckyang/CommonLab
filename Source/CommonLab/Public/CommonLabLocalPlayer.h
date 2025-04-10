// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "CommonLabLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:

	/**
	 * 로컬 플레이어가 호출되거나 플레이어 컨트롤러가 로컬 플레이어에 할당 될 때 호출,
	 */ 
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCommonLabLocalPlayer* LocalPlayer, APlayerController* PlayerController);
    FPlayerControllerSetDelegate OnPlayerControllerSet;
    
	/**
	 *  로컬 플레이에게 플레이어 상태가 할당횔 때 호출 됩니다.
	 *  서버에서 플에이어 스테이트를 복제 될때 호출 됩니다.
	 */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, UCommonLabLocalPlayer* LocalPlayer, APlayerState* PlayerState);
    FPlayerStateSetDelegate OnPlayerStateSet;
    
	/**
	 *  로컬 플에이어에게 플레이어 폰이 할당될 때 호출됩니다.
	 *  새로운 캐릭터로 변경 되거나 혹은, 특정 폰으로 빙의 되는 등 
	 */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPawnSetDelegate, UCommonLabLocalPlayer* LocalPlayer, APawn* Pawn);
    FPlayerPawnSetDelegate OnPlayerPawnSet;
};
