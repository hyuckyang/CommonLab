// Fill out your copyright notice in the Description page of Project Settings.


#include "Activatable/CommonLabActivatableStackable.h"

void UCommonLabActivatableStackable::SetPrevStackable(UCommonLabActivatableStackable* Stackable)
{
	if (Stackable)
	{
		PrevStackable = Stackable;
	}
}

