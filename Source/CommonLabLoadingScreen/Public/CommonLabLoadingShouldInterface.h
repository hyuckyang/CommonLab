// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonLabLoadingShouldInterface.generated.h"

// Common Loading Screen 의 ProcessInterface 와 완전히 동일한 인터페이스 입니다.
UINTERFACE()
class UCommonLabLoadingShouldInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONLABLOADINGSCREEN_API ICommonLabLoadingShouldInterface
{
	GENERATED_BODY()

public:
	// 로딩 중, 현재 해당 인터페이스를 상속받은 객체가 아직 로딩 중인지 체크 합니다.
	static bool ShouldShowLoadingScreen(UObject* TestObject);

	virtual bool ShouldShowLoadingScreen() const
	{
		return false;
	}
};
