// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLabDelegate.h"
#include "CommonLabSubClassInterface.h"
#include "CommonLabSubsystem.h"
#include "UObject/Object.h"
#include "CommonLabDelegateSubClass.generated.h"

// Template Parameter Pack String Macro
#define GET_PARAMS_STRING(...) GET_PARAMS_STRING_IMPL(__VA_ARGS__)
#define GET_PARAMS_STRING_IMPL(...) FString::Printf(TEXT("%s"), TEXT(#__VA_ARGS__))

/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabDelegateSubClass : public UObject, public ICommonLabSubClassInterface
{
	GENERATED_BODY()

public:

	template <typename... Params>
	static FCommonLabDelegate<Params...>* CLabAllocate(ULocalPlayer* LocalPlayer, UObject* Object, const FString& DelegateName)
	{
		if (UCommonLabDelegateSubClass* DelegateSubClass = LocalPlayer->GetGameInstance()->GetSubsystem<UCommonLabSubsystem>()->GetSubClass<UCommonLabDelegateSubClass>(LocalPlayer))
		{
			return DelegateSubClass->Allocate<Params... >(Object, DelegateName);
		}
		return nullptr;
	}

	static void CLabDeallocate(ULocalPlayer* LocalPlayer, UObject* Object)
	{
		if (UCommonLabDelegateSubClass* DelegateSubClass =  LocalPlayer->GetGameInstance()->GetSubsystem<UCommonLabSubsystem>()->GetSubClass<UCommonLabDelegateSubClass>(LocalPlayer))
		{
			DelegateSubClass->Deallocate(Object);
		}
	}
	
	// ~Begin ICommonLabSubClassInterface
	virtual void Initialize() override;
	virtual void Released() override;
	virtual void Destroyed() override;
	// ~End ICommonLabSubClassInterface

	template<typename... Params>
	FCommonLabDelegate<Params...>* Allocate(UObject* Object, const FString& DelegateName)
	{
		TSharedPtr<FCommonLabDelegate<Params...>> Delegate;

		FString UnifyName = GetDelegateName<Params...>(DelegateName);
		if (TSharedPtr<FCommonLabDelegateBase> Baesd = DelegateBasedMap.FindRef(UnifyName))
		{
			Delegate = StaticCastSharedPtr<FCommonLabDelegate<Params...>>(Baesd);
		}
		else
		{
			TSharedPtr<FCommonLabDelegate<Params...>> NewDelegate = MakeShared<FCommonLabDelegate<Params...>>(UnifyName);
			DelegateBasedMap.Add(UnifyName, NewDelegate);

			Delegate = NewDelegate;
		}

		Delegate->SetAllocateObject(Object);

		return Delegate;
	}

	void Deallocate(UObject* Object);
	void Collect();

	template <typename... Params>
	FCommonLabDelegate<Params...>* Find(const FString& DelegateName)
	{
		FString UnifyName = GetDelegateName<Params...>(DelegateName);
		if (TSharedPtr<FCommonLabDelegateBase> Based = DelegateBasedMap.FindRef(UnifyName))
		{
			return StaticCastSharedPtr<FCommonLabDelegate<Params...>>(Based).Get();
		}

		return nullptr;
	}

	template <typename... Params>
	void FindExecute(const FString& DelegateName, Params... Args)
	{
		if (FCommonLabDelegate<Params...> Delegate = Find<Params...>(DelegateName))
		{
			Delegate->Execute(Args...);
		}
	}
	
private:
	/*
	 * 추가 개발 사항, 인자의 순서와 상관이 없습니다. 즉 인자의 순서에 따른 DelegateName 를 내뱉는 작업을 별도로 진행해야 합니다. 
	 */
	template<typename... Params>
	static FString GetDelegateName(const FString& DelegateName)
	{
		return FString::Printf(TEXT("%s_%s"), *DelegateName, *GET_PARAMS_STRING(Params...));
	}
	
	TMap<FString, TSharedPtr<FCommonLabDelegateBase>> DelegateBasedMap;
};
