// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLabLocalPlayer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonLabSubsystem.generated.h"

class ICommonLabSubClassInterface;

/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// ~Begin UGameInstanceSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// ~End UGameInstanceSubsystem Interface

	// ~Begin Local Player Notification
	void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);
	// ~End Local Player Notification

#pragma region SubClass

protected:
	struct FLocalPlayerSubClass
	{
		TArray<TWeakInterfacePtr<ICommonLabSubClassInterface>> CLabSubClasses;
	};
	
	TMap<TWeakObjectPtr<UCommonLabLocalPlayer>, FLocalPlayerSubClass> CLabPlayerSubClasses;

	virtual void CreateSubClass(ULocalPlayer* LocalPlayer);
	void ForeachSubClass(ULocalPlayer* LocalPlayer, TFunction<void(ICommonLabSubClassInterface*)> Func);

	template<typename SubClassInterface = UObject>
	SubClassInterface* CreateSubClass(ULocalPlayer* LocalPlayer)
	{
		if (!LocalPlayer)
			return nullptr;

		UCommonLabLocalPlayer* CLabLocalPlayer = Cast<UCommonLabLocalPlayer>(LocalPlayer);
		if (!CLabLocalPlayer)
			return nullptr;

		if (CLabPlayerSubClasses.Contains(CLabLocalPlayer) == false)
		{
			FLocalPlayerSubClass LPlayerSubClass;
			CLabPlayerSubClasses.Add(CLabLocalPlayer, LPlayerSubClass);
		}

		// 생성 시 Interface 상속 받은 클래스 인지 체크
		if (TIsDerivedFrom<SubClassInterface, ICommonLabSubClassInterface>::IsDerived == false)
			return nullptr;
		
		FLocalPlayerSubClass& LocalPlayerSubClass = CLabPlayerSubClasses[CLabLocalPlayer];
		
		SubClassInterface* NewSubClass = NewObject<SubClassInterface>(LocalPlayer);	
		LocalPlayerSubClass.CLabSubClasses.Add(NewSubClass);

		
		return NewSubClass;
	}

public:
	template <typename SubClassInterface = UObject>
	SubClassInterface* GetSubClass(ULocalPlayer* LocalPlayer)
	{
		if (!LocalPlayer)
			return nullptr;

		UCommonLabLocalPlayer* CLabLocalPlayer = Cast<UCommonLabLocalPlayer>(LocalPlayer);
		if (!CLabLocalPlayer)
			return nullptr;

		if (!CLabPlayerSubClasses.Contains(CLabLocalPlayer))
			return nullptr;

		FLocalPlayerSubClass& LocalPlayerSubClass = CLabPlayerSubClasses[CLabLocalPlayer];
		for (const auto& SubClass : LocalPlayerSubClass.CLabSubClasses)
		{
			if (SubClass.IsValid())
			{
				UObject* SubClassObject = SubClass.GetObject();
				if (SubClassObject && SubClassObject->IsA(SubClassInterface::StaticClass()))
				{
					return Cast<SubClassInterface>(SubClassObject);
				}
			}
		}
		return nullptr;
	}
#pragma endregion 
	
};
