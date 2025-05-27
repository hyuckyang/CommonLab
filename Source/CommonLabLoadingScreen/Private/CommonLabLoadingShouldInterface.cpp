// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLabLoadingShouldInterface.h"


// 타겟 오브젝트에 ICommonLabLoadingShouldInterface 인터페이스를 상속 받았다면 , 검사합니다.
bool ICommonLabLoadingShouldInterface::ShouldShowLoadingScreen(UObject* TestObject)
{
	if(TestObject != nullptr)
	{
		if (const ICommonLabLoadingShouldInterface* LoadObserver = Cast<ICommonLabLoadingShouldInterface>(TestObject))
		{
			FString ObserverReason;
			if (LoadObserver->ShouldShowLoadingScreen())
			{
				return true;
			}
		}
	}
	return false;
}
