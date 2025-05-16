// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLabPreLoadingScreenSetting.h"

UCommonLabPreLoadingScreenSetting::UCommonLabPreLoadingScreenSetting()
{
	CategoryName = TEXT("Game");
}

void UCommonLabPreLoadingScreenSetting::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 테스트 중
	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCommonLabPreLoadingScreenSetting, PreLoadingCircleThrobberImage))
	{
		// 이미지가 비어있지 않으면 true로 설정
		bIsPreLoadingCircleThrobber = PreLoadingCircleThrobberImage.GetResourceObject() != nullptr;
	}
}
