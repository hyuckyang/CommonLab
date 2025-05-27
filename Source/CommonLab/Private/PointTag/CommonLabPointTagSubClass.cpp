// Copyright Evr studio, Inc. All Rights Reserved.

#include "PointTag/CommonLabPointTagSubClass.h"

void UCommonLabPointTagSubClass::Initialize()
{
}

void UCommonLabPointTagSubClass::Released()
{
	for (auto& Pair : DelegateMap)
	{
		if (Pair.Value.IsValid())
		{
			Pair.Value->Clear();
		}
	}
	DelegateMap.Empty();
}

void UCommonLabPointTagSubClass::Destroyed()
{
}
