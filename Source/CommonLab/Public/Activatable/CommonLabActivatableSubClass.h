// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonLabActivatableWidget.h"
#include "CommonLabSubClassInterface.h"
#include "CommonLabActivatableStackable.h"
#include "CommonLabActivatableSubClass.generated.h"

class UOverlay;

// USTRUCT()
// struct FStackableTagKey
// {
// 	GENERATED_BODY()
//
// 	// 중요한 것은 Tag 입니다. Order 는 필요에 의해 생성된 변수입니다.
// 	FGameplayTag Tag;
// 	int32 Order;
//
// 	FStackableTagKey(int32 InOrder, FGameplayTag InTag)
// 		: Tag(InTag)
// 		, Order(InOrder)
// 	{
// 	}
//
// 	
// 	bool operator<(const FStackableTagKey& Other) const
// 	{
// 		return Tag == Other.Tag;
// 	}
// };

/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabActivatableSubClass : public UObject, public ICommonLabSubClassInterface
{
	GENERATED_BODY()

public:

	// ~Begin ICommonLabSubClassInterface
	virtual void Initialize() override;
	virtual void Released() override;
	virtual void Destroyed() override;
	// ~End ICommonLabSubClassInterface


	template <typename Activatable = UCommonLabActivatableWidget>
	Activatable* Open(FGameplayTag Tag, FString Path)
	{
		const FSoftClassPath ActivatableClassRef(*Path);
		if (UClass* ActivatableClass = ActivatableClassRef.TryLoadClass<UCommonLabActivatableWidget>())
		{
			Open<Activatable>(Tag, ActivatableClass);
		}
		return nullptr;
	}

	template <typename Activatable = UCommonLabActivatableWidget>
	Activatable* Open(FGameplayTag Tag, UClass* ActivatableClass)
	{
		return Open<Activatable>(Tag, ActivatableClass, [](Activatable&){});
	}

	template <typename Activatable = UCommonLabActivatableWidget>
	Activatable* Open(FGameplayTag Tag, UClass* ActivatableClass, TFunctionRef<void(Activatable&)> Function)
	{
		// 선언한 클래스가 UCommonLabActivatableWidget 상속받지 않는 객체라면
		if (TIsDerivedFrom<Activatable, UCommonLabActivatableWidget>::IsDerived == false)
			return nullptr;

		if (!Root.IsValid() || ActivatableClass == nullptr)
			return nullptr;

		if (TWeakObjectPtr<UCommonLabActivatableStackable> Stackable = ActivatableStacks.FindRef(Tag); Stackable.IsValid())
		{
			Activatable* ActivatableCreate = Stackable->AddWidget<Activatable>(ActivatableClass, Function);
			if (UCommonLabActivatableWidget* LabActivatable = Cast<UCommonLabActivatableWidget>(ActivatableCreate))
			{
				LabActivatable->SetActivatedStackable(Stackable.Get());
			}
			return ActivatableCreate;
		}
		
		return nullptr;
	}

private:

	void AddRootFromViewport();
	void RemoveRootFromViewport();
	void DestroyRootFromViewport();

	UCommonLabActivatableStackable* AddStackable(FGameplayTag Tag);
	void AddActivatableClass();

	/*
	 * Add to Viewport 를 동해 뷰포트에 출력되는 Root 위젯 , Root 아래에 Tag 별로 Stackable 이 생성되고 그 아래 실제 화면에 보여지는 UI 가 생성됩니다.
	 */
	TWeakObjectPtr<UUserWidget> Root;
	TWeakObjectPtr<UOverlay> RootOverlay;

	// GameplayTag 별로 Stackable 을 생성
	// 
	TMap<FGameplayTag, TWeakObjectPtr<UCommonLabActivatableStackable>> ActivatableStacks;
};
