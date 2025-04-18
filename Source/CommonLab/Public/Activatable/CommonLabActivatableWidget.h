// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonLabActivatableWidget.generated.h"

class UCommonLabActivatableStackable;

/*
 * Lyra 샘플의 ELyraWidgetInputMode 를 그대로 활용하였습니다.  
 */
UENUM(BlueprintType)
enum class ECommonLabActivatableInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * Lyra 샘플 의 ULyraActivatableWidget 를 그대로 활용하였습니다.
 * 아래와 같은 정의를 통해, 별도의 로직 없이, InputConfig/GameMouseCaptureMode 조작하면 입력값에 대한 전달을 통제할 수 있습니다.
 */
UCLASS()
class COMMONLAB_API UCommonLabActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UCommonLabActivatableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {};

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	// ~Begin Overlap , 다른 창이 본인 창으로 올라올 때, 혹은 사라질 때 호출 됩니다.
	UFUNCTION(BlueprintImplementableEvent, Category=CommonLabActivatable, meta =(DisplayName="OnOverlap"))
	void BP_OnOverlap(UCommonLabActivatableWidget* OtherWidget, bool bIsOverlapping);
	virtual void NativeOnOverlap(UCommonLabActivatableWidget* OtherWidget, bool bIsOverlapping);
	// ~End Overlap
	
	// ~Begin UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;	
	// ~End UCommonActivatableWidget interface

	// 셋팅
	void SetActivatableStackable(UCommonLabActivatableStackable* Stackable);
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = CommonLabInput)
	ECommonLabActivatableInputMode InputConfig = ECommonLabActivatableInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = CommonLabInput)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;

	//
	TWeakObjectPtr<UCommonLabActivatableStackable> ActivatableStackable;
	
};
