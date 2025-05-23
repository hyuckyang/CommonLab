// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonLabLoadingWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCommonLabLoadingWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 로딩창 위젯이 해당 인터페이스를 상속 받았다면
 * Key 입력에 대한 반응을 할 수 있겠금 유도하는 인터페이스 입니다.
 * 아무키 혹은 특정키 입력(누름)에 대한 반응을 유도 합니다.
 */
class COMMONLABLOADINGSCREEN_API ICommonLabLoadingWidgetInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKeyPressedEvent(const FKeyEvent& InKeyEvent);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnKeyReleaseEvent(const FKeyEvent& InKeyEvent);
};
