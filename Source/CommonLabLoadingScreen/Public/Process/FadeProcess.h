// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FadeProcess.generated.h"

/**
 * 
 */
UCLASS()
class UFadeProcess : public UObject
{
	GENERATED_BODY()

protected:
	enum EFadeProcess
	{
		None,
		Start,
		Tick,
		Complete,
	};

	bool bIsFade = false;
	FLinearColor FadeColorFrom = FLinearColor::Black;
	FLinearColor FadeColorTo = FLinearColor::Black;
	EFadeProcess FadeProcess = None;

	float Elapsed = 0.f;
	float Duration = 0.f;
	
	TSharedPtr<SCompoundWidget> SFadeWidget;

public:
	virtual void Clean();
	// 
	virtual void FadeFunc(bool bFadeOut, float Transition, FLinearColor FadeColor);
	virtual void FadeFunc(bool bFadeOut, float Transition, FLinearColor FadeFromColor, FLinearColor FadeToColor);
	virtual bool FadeTick(float DeltaTime);

protected:

	void SetViewportFadeWidget(bool bIsShow);
	/*
	 * 기본으로 SCommonLabLoadingFadeSWidget 생성합니다.
	 * 다만 WidgetClass 시 이를 대체할 수 있습니다.
	 * UUserWidget* 에 대해서는 SWidget 의 형변환으로 하는 방법을 찾는다면 수정 될 예정입니다.
	 */
	UUserWidget* SetViewportWidget(bool bIsShow, TSharedPtr<SWidget>& SWidget, const TSubclassOf<UUserWidget>& WidgetClass, int32 ZOrder) const;
};