// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLabPlayerController.h"
#include "CommonLabLocalPlayer.h"

void ACommonLabPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	// 다시 플레이어 에 컨트롤러에 부여
	if (UCommonLabLocalPlayer* LocalPlayer = Cast<UCommonLabLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);

		if (PlayerState)
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}

void ACommonLabPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (UCommonLabLocalPlayer* LocalPlayer = Cast<UCommonLabLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, InPawn);
	}
	
}

void ACommonLabPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (UCommonLabLocalPlayer* LocalPlayer = Cast<UCommonLabLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, InPawn);
	}
}

void ACommonLabPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	if (UCommonLabLocalPlayer* LocalPlayer = Cast<UCommonLabLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, nullptr);
	}
}

void ACommonLabPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (PlayerState)
	{
		if (UCommonLabLocalPlayer* LocalPlayer = Cast<UCommonLabLocalPlayer>(Player))
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);	
		}
	}
}



