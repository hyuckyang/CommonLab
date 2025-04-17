// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonLabSubsystem.h"
#include "CommonLabSubClassInterface.h"
#include "Delegate/CommonLabDelegateSubClass.h"
#include "Activatable/CommonLabActivatableSubClass.h"

void UCommonLabSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCommonLabSubsystem::Deinitialize()
{
	Super::Deinitialize();

	// CLabPlayerSubClasses 맵을 순회하며 각 SubClass를 해제하고 맵에서 제거
	for (auto& CLabPlayerSubClassPair : CLabPlayerSubClasses)
	{
		if (UCommonLabLocalPlayer* CLabLocalPlayer = CLabPlayerSubClassPair.Key.Get())
		{
			FLocalPlayerSubClass& LocalPlayerSubClass = CLabPlayerSubClassPair.Value;
  
			// LocalPlayerSubClass.CLabSubClasses 순회하며 Destroyed 호출
			for (auto& SubClass : LocalPlayerSubClass.CLabSubClasses)
			{
				if (SubClass.IsValid())
				{
					if (ICommonLabSubClassInterface* SubClassInterface = SubClass.Get())
					{
						SubClassInterface->Destroyed();
					}
				}
			}
  
			// CLabSubClasses 비우기
			LocalPlayerSubClass.CLabSubClasses.Empty();
		}
	}
 

	// 맵 비우기
	CLabPlayerSubClasses.Empty();
}

// 서브시스템의 생성 여부를 결정
bool UCommonLabSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// Outer(GameInstance) 가 서버 인스턴스가 아닌 경우	
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		// GetDerivedClasses : 해당 클래스를 상속받은 클래스들을 가져온다.
		// ChildClasses.Num() == 0 : 해당 클래스를 상속받은 클래스가 없는 경우
		GetDerivedClasses(GetClass(), ChildClasses, false);
		return ChildClasses.Num() == 0;
	}
	return false;
}

void UCommonLabSubsystem::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (UCommonLabLocalPlayer* CLabLocalPlayer = Cast<UCommonLabLocalPlayer>(LocalPlayer))
	{
		if (!CLabLocalPlayer)
			return ;

		CreateSubClass(LocalPlayer);

		if (CLabLocalPlayer->OnPlayerControllerSet.IsBound() == false)
        {
			CLabLocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UCommonLabLocalPlayer* LPlayer, APlayerController* PlayerController)
			{
				ForeachSubClass(LPlayer, [](ICommonLabSubClassInterface* SubClass)
				{
					// 해체
					SubClass->Released();

					//-> 초기화
					SubClass->Initialize();
				});
			});
        }

		ForeachSubClass(LocalPlayer, [](ICommonLabSubClassInterface* SubClass)
		{
			// 초기화
			SubClass->Initialize();
		});
	}
}

void UCommonLabSubsystem::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	ForeachSubClass(LocalPlayer, [](ICommonLabSubClassInterface* SubClass)
	{
		// 해체
		SubClass->Released();
	});
}

void UCommonLabSubsystem::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	UCommonLabLocalPlayer* CLabLocalPlayer = Cast<UCommonLabLocalPlayer>(LocalPlayer);
	if (!CLabLocalPlayer)
		return;

	NotifyPlayerRemoved(LocalPlayer);
	
	CLabLocalPlayer->OnPlayerControllerSet.RemoveAll(this);

	ForeachSubClass(LocalPlayer, [](ICommonLabSubClassInterface* SubClass)
	{
		// 해체
		SubClass->Destroyed();
	});

	if (CLabPlayerSubClasses.Contains(CLabLocalPlayer))
	{
		// 서브클래스 삭제
		CLabPlayerSubClasses[CLabLocalPlayer].CLabSubClasses.Empty();
	
		// 플레이어 삭제 
		CLabPlayerSubClasses.Remove(CLabLocalPlayer);	
	}
}

void UCommonLabSubsystem::CreateSubClass(ULocalPlayer* LocalPlayer)
{
	CreateSubClass<UCommonLabDelegateSubClass>(LocalPlayer);
	CreateSubClass<UCommonLabActivatableSubClass>(LocalPlayer);
}

void UCommonLabSubsystem::ForeachSubClass(ULocalPlayer* LocalPlayer, TFunction<void(ICommonLabSubClassInterface*)> Func)
{
	UCommonLabLocalPlayer* CLabLocalPlayer = Cast<UCommonLabLocalPlayer>(LocalPlayer);
	if (!CLabLocalPlayer)
		return;

	if (CLabPlayerSubClasses.Contains(CLabLocalPlayer))
	{
		for (const auto& SubClass : CLabPlayerSubClasses[CLabLocalPlayer].CLabSubClasses)
		{
			if (ICommonLabSubClassInterface* SubClassInterface = SubClass.Get())
			{
				Func(SubClassInterface);
			}
		}
	}
}





