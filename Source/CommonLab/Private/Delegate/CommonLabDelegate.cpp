// Fill out your copyright notice in the Description page of Project Settings.


#include "Delegate/CommonLabDelegate.h"

#pragma region Delegate Base

void FCommonLabDelegateBase::SetAllocateObject(UObject* Object, bool bAllocate)
{
	if(bAllocate)
	{
		if(!AllocateObjects.Contains(Object))
		{
			AllocateObjects.Add(Object);
		}
	}
	else
	{
		if(AllocateObjects.Contains(Object))
		{
			AllocateObjects.Remove(Object);
		}
	}
}

void FCommonLabDelegateBase::CleanUpNullAllocateObject()
{
	AllocateObjects.RemoveAll([](TWeakObjectPtr<UObject> AObject)
	{
		return AObject == nullptr;
	});
}

#pragma endregion
