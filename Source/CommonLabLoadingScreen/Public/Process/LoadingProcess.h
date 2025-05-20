// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FadeProcess.h"
#include "UObject/Object.h"
#include "LoadingProcess.generated.h"

/**
 * 
 */
UCLASS()
class ULoadingProcess : public UFadeProcess
{
	GENERATED_BODY()

private:

	enum ELoadProcess
	{
		None,
		Start,
		Tick,
		LoadWaitFrame, // 로드 대기 ( Open Level 등 무엇이든 1프레임 대기 )
		Load, // 로드 ( Open Level 등 )
		Complete
	};

	ELoadProcess LoadProcess = None;

	UPROPERTY()
	TSubclassOf<UUserWidget> LoadSubClass;
	TWeakObjectPtr<UUserWidget> LoadWidget;

	TDelegate<void()> CompleteDelegate;
	
public:
	virtual void Clean() override;
	virtual bool FadeTick(float DeltaTime) override;

	void LoadStart(float Transition, TSubclassOf<UUserWidget> WidgetSubClass, FLinearColor Color = FLinearColor(0.f, 0.f, 0.f, 1.f));
	void LoadEnd();

private:
	void SetViewportLoadWidget(bool bIsShow);
};
