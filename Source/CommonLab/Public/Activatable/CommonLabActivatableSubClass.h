// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonLabActivatableWidget.h"
#include "CommonLabSubClassInterface.h"
#include "CommonLabActivatableStackable.h"
#include "CommonLabSubsystem.h"
#include "CommonLabActivatableSubClass.generated.h"

class UOverlay;
class UCommonUserWidget;

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
		UE_LOG(LogTemp, Display, TEXT("UCommonLabActivatableSubClass::Open"));
		
		// 선언한 클래스가 UCommonLabActivatableWidget 상속받지 않는 객체라면
		if (TIsDerivedFrom<Activatable, UCommonLabActivatableWidget>::IsDerived == false)
			return nullptr;

		if (Root == nullptr || ActivatableClass == nullptr)
			return nullptr;

		if (TWeakObjectPtr<UCommonLabActivatableStackable> Stackable = ActivatableStacks.FindRef(Tag); Stackable.IsValid())
		{
			auto LambInitialize = [Stackable, Function](Activatable& Widget)
			{
				Function(Widget);
				if (UCommonLabActivatableWidget* ActivatableWidget = Cast<UCommonLabActivatableWidget>(&Widget))
				{
					ActivatableWidget->SetActivatableStackable(Stackable.Get());
				}
			};

			
			return Stackable->AddWidget<Activatable>(ActivatableClass, LambInitialize);
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
	UPROPERTY()
	TObjectPtr<UUserWidget> Root;
	
	UPROPERTY()
	TObjectPtr<UOverlay> RootOverlay;

	// GameplayTag 별로 Stackable 을 생성
	// 
	TMap<FGameplayTag, TWeakObjectPtr<UCommonLabActivatableStackable>> ActivatableStacks;
};
