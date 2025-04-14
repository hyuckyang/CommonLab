// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class COMMONLAB_API FCommonLabDelegateBase
{
public:
	FCommonLabDelegateBase(const FString& DelegateName) : BasedName(DelegateName) {}
	virtual ~FCommonLabDelegateBase() = default;
	virtual void Clear() { AllocateObjects.Empty(); }

	// FCommonLabDelegate 클래스 내부에 존재해야 하나
	// FCommonLabDelegateBase 를 호출하여 사용될 수 있어 이곳에 virtual 로 선언합니다.
	virtual void RemoveFunction(UObject* Object) = 0;
	
	// Object 대입 
	void SetAllocateObject(UObject* Object, bool bAllocate = true);
	int32 GetAllocateObjectsCount() const { return AllocateObjects.Num(); }

	// Nullptr Object 정리
	void CleanUpNullAllocateObject();
	
protected:
	FString BasedName;
	
private:
	TArray<TWeakObjectPtr<UObject>> AllocateObjects;
};

template <typename... Params>
class FCommonLabDelegate : public FCommonLabDelegateBase
{

public:
	FCommonLabDelegate(const FString& DelegateName) : FCommonLabDelegateBase(DelegateName) {};
	virtual ~FCommonLabDelegate() override
	{
		FCommonLabDelegateBase::Clear();
	}

	virtual void Clear() override
	{
		FCommonLabDelegateBase::Clear();

		FuncDelegate.Clear();
	}

	virtual void RemoveFunction(UObject* Object) override
	{
		FuncDelegate.RemoveAll(Object);
	}
	
	template <typename UserClass = UObject>
	void AddFunction(UserClass* Object, typename TMemFunPtrType<false, UserClass, void(Params...)>::Type Func)
	{
		FuncDelegate.AddUObject(Object, Func);
	}

	void Execute(Params... Args)
	{
		if (FuncDelegate.IsBound())
			FuncDelegate.Broadcast(Args...);
	}
	
private:
	TMulticastDelegate<void(Params...)> FuncDelegate;
};
