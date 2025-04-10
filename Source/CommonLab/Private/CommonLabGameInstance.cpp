// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLabGameInstance.h"

#include "CommonLabSubsystem.h"


int32 UCommonLabGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 Val =  Super::AddLocalPlayer(NewPlayer, UserId);
	{
		 // TODO ..
	}
	
	GetSubsystem<UCommonLabSubsystem>()->NotifyPlayerAdded(NewPlayer);

	return Val;
}

bool UCommonLabGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	GetSubsystem<UCommonLabSubsystem>()->NotifyPlayerDestroyed(ExistingPlayer);
	
	return Super::RemoveLocalPlayer(ExistingPlayer);
}

