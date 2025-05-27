// Copyright Evr studio, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Overlay.h"
#include "CommonLabPointTagWidget.generated.h"

class UCommonLabPointTagSubClass;

/**
 * CommonLabPointTagWidget
 * 0. Lyra CommonGame 플러그인 중, UIExtension의 UUIExtensionPointWidget을 참고 하였습니다. (Tag 관리)
 * 1. Tag로 위젯을 관리되는 대상(위젯)이 됩니다. (UCommonLabPointTagSubClass)
 * 2. Tag로 위젯이 관리되어 Visible을 관리합니다. (Push Pop)
 * 3. 추가적인 확장 기능을 위한 기반 클래스를 목표로 합니다
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCommonLabPointWidgetVisible, class UCommonLabPointTagWidget*, PointWidget, bool, Visible);

UCLASS()
class COMMONLAB_API UCommonLabPointTagWidget : public UOverlay
{
	GENERATED_BODY()

public:
	// 위젯이 파괴되거나 화면에서 제거 될 때, 즉 Slate 에서 필요치 않을 때 호출 됩니다.
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	// 위젯의 Slate 요소를 처음 또는 다시 구축시 호출 됩니다.
	// 예를 들면 위젯이 처음 생성되거나 레이아웃이나 스타일 변경되어 UI를 다시 그릴 때 호출 됩니다.
	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:

	virtual void Allocate(UCommonLabPointTagSubClass* TagSubClass);
	virtual void Deallocate(UCommonLabPointTagSubClass* TagSubClass);

private:
	void Allocate();
	void Deallocate();

protected:
	
	UPROPERTY(EditAnywhere)
	FGameplayTag PointTag;
	
#pragma region Visible 관리

private:
	
	bool bWasEmpty = false;

	struct FDisableState
	{
		int Key;
		bool bIsDisable;
		TWeakObjectPtr<UObject> TargetObject;

		FDisableState(int InKey, bool bInIsEnable, UObject* InObject)
			: Key(InKey)
			, bIsDisable(bInIsEnable)
			, TargetObject(InObject)
		{
		}

		FDisableState()
			: Key(0), bIsDisable(false), TargetObject(nullptr)
		{	
		}

		bool operator==(const UObject* OtherObjet) const { return TargetObject != nullptr && TargetObject == OtherObjet; }
	};
	TArray<FDisableState> PushToDisableObjects;
	 
	void OnPushToVisible(UObject* Object, bool bIsVisible);

protected:

	/*
	 * Visible 을 시도 할 때, 호출 되는 델리게이트
	 */
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true, DisplayName = "On Poinit Widget Visible"))
	FCommonLabPointWidgetVisible OnPointWidgetVisible;
	
#pragma endregion 

	
	
};
