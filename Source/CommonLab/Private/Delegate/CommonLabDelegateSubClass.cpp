// Fill out your copyright notice in the Description page of Project Settings.


#include "Delegate/CommonLabDelegateSubClass.h"

void UCommonLabDelegateSubClass::Initialize()
{
	Collect();
}

void UCommonLabDelegateSubClass::Released()
{
	Collect();
}

void UCommonLabDelegateSubClass::Destroyed()
{
	// 모두 초기화 
	for (auto Iter = DelegateBasedMap.CreateIterator(); Iter; ++Iter)
	{
		TSharedPtr<FCommonLabDelegateBase> Base = Iter->Value;
		Base.Reset();
	}

	DelegateBasedMap.Empty();
}

void UCommonLabDelegateSubClass::Deallocate(UObject* Object)
{
	auto Iter = DelegateBasedMap.CreateIterator();
	while (Iter)
	{
		TSharedPtr<FCommonLabDelegateBase> Base = Iter->Value;

		// 해당 객체를 통해 등록된 Delegate 를 모두 삭제하고
		// 해당 객체를 Delegate AllocateObject 에서 삭제합니다.
		Base->RemoveFunction(Object);
		Base->SetAllocateObject(Object, false);

		// 초기화
		Base->CleanUpNullAllocateObject();
		if (Base->GetAllocateObjectsCount() <= 0)
		{
			Iter.RemoveCurrent();
		}
		
		++Iter;
	}
}

void UCommonLabDelegateSubClass::Collect()
{
	// null 객체 정리 및 정리
	for (auto Iter = DelegateBasedMap.CreateIterator(); Iter; ++Iter)
	{
		TSharedPtr<FCommonLabDelegateBase> Base = Iter->Value;
		Base->CleanUpNullAllocateObject();

		if (Base->GetAllocateObjectsCount() <= 0)
		{
			Base.Reset();
			Iter.RemoveCurrent();
		}
		else
		{
			++Iter; // Iter.RemoveCurrent()를 호출하지 않는 경우에만 이터레이터를 증가시킵니다.
		}
	}
}


