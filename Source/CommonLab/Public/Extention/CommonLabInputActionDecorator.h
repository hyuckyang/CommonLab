// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "CommonLabInputActionDecorator.generated.h"

class UDataTable;
class UCommonInputSubsystem;
/**
 * 
 */
UCLASS()
class COMMONLAB_API UCommonLabInputActionDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	UCommonLabInputActionDecorator(const FObjectInitializer& ObjectInitializer);

	virtual void BeginDestroy() override;
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	
	virtual const FSlateBrush* FindImageBrush(FName TagOrId, bool bWarnIfMissing);
	FVector2D GetIconLocation(int32 FontSize);

protected:

	TMap<FName, FSlateBrush> Icons;
	UCommonInputSubsystem* GetInputSubsystem() const;

	//  런타임이 아닌 에디터에서 편집시 보여주는 아이콘
	UPROPERTY(EditAnywhere, Category="Editor FSlate Brush")
	FSlateBrush SlateBrush;
	
	UPROPERTY(EditAnywhere, Category = "CommonInput", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	UDataTable* CommonInputActionDataTable;

	UPROPERTY(EditAnywhere, Category = "CommonInput")
	FVector2D CommonActionIconLocation;

	// 폰트 사이트 에 따른 아이콘 위치 설정
	UPROPERTY(EditAnywhere, Category = "CommonInput")
	TMap<int32, FVector2D> CommonActionIconLocationByFontSize;

private:
	TWeakObjectPtr<URichTextBlock> OwnerRichTextBlock;
};
