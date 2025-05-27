// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FadeProcess.h"
#include "UObject/Object.h"
#include "LoadingProcess.generated.h"

class IInputProcessor;
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
	TDelegate<bool()> LoadCompleteDelegate;
	
	TSharedPtr<IInputProcessor> InputProcessor;
public:
	virtual void Clean() override;
	virtual bool FadeTick(float DeltaTime) override;

	void LoadStart(float Transition, const TSubclassOf<UUserWidget>& WidgetSubClass, FLinearColor Color, const TDelegate<bool()>& LoadDelegate);
	void LoadStart(float Transition, const TSubclassOf<UUserWidget>& WidgetSubClass, FLinearColor FadeFromColor,  FLinearColor FadeToColor, const TDelegate<bool()>& LoadDelegate);
	void LoadEnd();

	bool IsLoadProcess() const { return LoadProcess == LoadWaitFrame || LoadProcess == Load; }

private:
	
	void SetViewportLoadWidget(bool bIsShow);
	void SetBlockingInput(bool bIsInputBlock); /* true -> 입력 안됨(FLoadingProcessInputPreProcess 에 권한 넘김), false -> 입력 됨  */
};
