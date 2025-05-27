// Copyright Evr studio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonLabSubClassInterface.h"
#include "GameplayTagContainer.h"
#include "Delegate/CommonLabDelegate.h"
#include "UObject/NoExportTypes.h"
#include "CommonLabPointTagSubClass.generated.h"

class UCommonLabPointTagWidget;

/**
 * CommonLabPointTagSubClass
 * 
 * PointTag 시스템에서 관리되는 서브클래스의 기본 클래스입니다.
 * Tag 로 출력된 UI 를 찾아냅니다
 * Tag 로 찾아낸 위젯에 특정 기능을 실행합니다.
 * 위젯의 특정 기능은 CommonLabDelegate 를 이용합니다. ( 이름 과 타입으로 설정 )
 */
UCLASS(Abstract, Blueprintable)
class COMMONLAB_API UCommonLabPointTagSubClass : public UObject, public ICommonLabSubClassInterface
{
	GENERATED_BODY()

public:
	
	// ~Begin ICommonLabSubClassInterface
	virtual void Initialize() override;
	virtual void Released() override;
	virtual void Destroyed() override;
	// ~End ICommonLabSubClassInterface

private:

	// Common Lab Delegate 를 이용하여. PointWidget 의 다양한 기능을 등록합니다.
	// Common Lab Delegate SubClass 를 통한 관리는 하지 않습니다.
	struct FDelegateKey
	{
		FGameplayTag Tag;
		FString Name;

		FDelegateKey(FGameplayTag InTag, const FString& InName)
			: Tag(InTag), Name(InName)
		{
		}

		bool operator == (const FDelegateKey& Other) const
		{
			return Tag == Other.Tag && Name == Other.Name;
		}

		friend uint32 GetTypeHash(const FDelegateKey& Key)
		{
			return HashCombine(GetTypeHash(Key.Tag), GetTypeHash(Key.Name));
		}
	};

	TMap<FDelegateKey, TSharedPtr<FCommonLabDelegateBase>> DelegateMap;

public:

	template<typename... Params>
	FCommonLabDelegate<Params...>* Allocate(const FGameplayTag Tag, UObject* PointObject, const FString& DelegateName)
	{
		FDelegateKey Key(Tag, DelegateName);
		if (TSharedPtr<FCommonLabDelegateBase>* FoundDelegate = DelegateMap.Find(Key))
		{
			return static_cast<FCommonLabDelegate<Params...>*>(FoundDelegate->Get());
		}

		TSharedPtr<FCommonLabDelegate<Params...>> MakeDelegate = MakeShared<FCommonLabDelegate<Params...>>(DelegateName);
		MakeDelegate->SetAllocateObject(PointObject);

		DelegateMap.Add(Key, MakeDelegate);

		return MakeDelegate.Get();
	}

	void Deallocate(FGameplayTag Tag, UObject* PointObject, const FString& DelegateName)
	{
		FDelegateKey Key(Tag, DelegateName);
		if (TSharedPtr<FCommonLabDelegateBase>* DelegatePtr = DelegateMap.Find(Key))
		{
			// 사실상 UCommonLabDelegateSubClass 의 Deallocate 와 동일 합니다.
			if (FCommonLabDelegateBase* DelegateBase = DelegatePtr->Get())
			{
				DelegateBase->RemoveFunction(PointObject);
				DelegateBase->SetAllocateObject(PointObject, false);
				DelegateBase->CleanUpNullAllocateObject();

				if (DelegateBase->GetAllocateObjectsCount() <= 0)
				{
					// 할당된 객체가 없으면 DelegateMap에서 제거합니다.
					DelegateMap.Remove(Key);
				}
			}
		}
	}
};
