// Fill out your copyright notice in the Description page of Project Settings.


#include "Extention/CommonLabInputActionDecorator.h"
#include "Fonts/FontMeasure.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SBox.h"
#include "Misc/DefaultValueHelper.h"
#include "CommonUITypes.h"
#include "CommonInputSubsystem.h"
#include "Components/RichTextBlock.h"

#pragma region Slate
class SCommonInputActionIconInlineImage : public SCompoundWidget
{
protected:
	const FSlateBrush* Brush = nullptr;
	TOptional<int32> Width;
	TOptional<int32> Height;
	FVector2D RendLocation;
	

public:
	SLATE_BEGIN_ARGS(SCommonInputActionIconInlineImage)
		{}
		SLATE_ARGUMENT(const FSlateBrush*, Brush)
		SLATE_ARGUMENT(TOptional<int32>, Width)
		SLATE_ARGUMENT(TOptional<int32>, Height)
		SLATE_ARGUMENT(FVector2D, RendLocation)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const FTextBlockStyle& TextStyle, EStretch::Type Stretch)
	{
		Brush = InArgs._Brush;
		Width = InArgs._Width;
		Height = InArgs._Height;
		RendLocation = InArgs._RendLocation;
		
		// 현재 텍스트의 폰트 정보 중, 폰트 크기 ( 높이 ) 와 이미지 높이중 가장 작은 정보를 가져옵니다.
		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		float IconHeight = FMath::Min((float)FontMeasure->GetMaxCharacterHeight(TextStyle.Font, 1.0f), Brush->ImageSize.Y);

		// 의미 있는 값이 설정 되었다면.
		if (Height.IsSet())
		{
			IconHeight = Height.GetValue();
		}

		float IconWidth = IconHeight;
		if (Width.IsSet())
		{
			IconWidth = Width.GetValue();
		}
		
		ChildSlot
		[
			SNew(SBox)
			.HeightOverride(IconHeight)
			.WidthOverride(IconWidth)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				.StretchDirection(EStretchDirection::DownOnly)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SImage)
					.Image(Brush)
					.RenderTransform(FSlateRenderTransform(RendLocation))
				]
			]
		];
	}
};

class FCommonInputActionIconInlineImage : public FRichTextDecorator
{
public:
	FCommonInputActionIconInlineImage(URichTextBlock* InOwner, UCommonLabInputActionDecorator* InDecorator)
		: FRichTextDecorator(InOwner)
		  , Decorator(InDecorator)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		if (RunParseResult.Name == TEXT("img") && RunParseResult.MetaData.Contains(TEXT("id")))
		{
			const FTextRange& IdRange = RunParseResult.MetaData[TEXT("id")];
			const FString TagId = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);

			return Decorator->FindImageBrush(*TagId, false) != nullptr;
		}
		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		TOptional<int32> Width;
		if (const FString* WidthString = RunInfo.MetaData.Find(TEXT("width")))
		{
			int32 WidthTemp;
			Width = FDefaultValueHelper::ParseInt(*WidthString, WidthTemp) ? WidthTemp : TOptional<int32>();
		}

		TOptional<int32> Height;
		if (const FString* HeightString = RunInfo.MetaData.Find(TEXT("height")))
		{
			int32 HeightTemp;
			Height = FDefaultValueHelper::ParseInt(*HeightString, HeightTemp) ? HeightTemp : TOptional<int32>();
		}

		EStretch::Type Stretch = EStretch::ScaleToFit;
		if (const FString* SstretchString = RunInfo.MetaData.Find(TEXT("stretch")))
		{
			static const UEnum* StretchEnum = StaticEnum<EStretch::Type>();
			int64 StretchValue = StretchEnum->GetValueByNameString(*SstretchString);
			if (StretchValue != INDEX_NONE)
			{
				Stretch = static_cast<EStretch::Type>(StretchValue);
			}
		}

		const FString TagId = RunInfo.MetaData[TEXT("id")];
		const FSlateBrush* ImageBrush = Decorator->FindImageBrush(*TagId, true);
		FVector2D ImageLocation = Decorator->GetIconLocation(TextStyle.Font.Size);
		
		return SNew(SCommonInputActionIconInlineImage, TextStyle, Stretch)
			.Brush(ImageBrush)
			.Width(Width)
			.Height(Height)
		    .RendLocation(ImageLocation);
	}

private:
	UCommonLabInputActionDecorator* Decorator;
};
#pragma endregion

UCommonLabInputActionDecorator::UCommonLabInputActionDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CommonInputActionDataTable(nullptr)
{
}

void UCommonLabInputActionDecorator::BeginDestroy()
{
	Icons.Empty();
	Super::BeginDestroy();
}


TSharedPtr<ITextDecorator> UCommonLabInputActionDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	OwnerRichTextBlock = InOwner;
	if (OwnerRichTextBlock.IsValid())
	{
		if (UCommonInputSubsystem* CommonInput = GetInputSubsystem())
		{
			CommonInput->OnInputMethodChangedNative.AddUObject(this, &UCommonLabInputActionDecorator::OnInputMethodChanged);
		}
	}


	
	return MakeShareable(new FCommonInputActionIconInlineImage(InOwner, this));
}

const FSlateBrush* UCommonLabInputActionDecorator::FindImageBrush(FName TagOrId, bool bWarnIfMissing)
{
	// 예외처리 중,
	if (OwnerRichTextBlock == nullptr || OwnerRichTextBlock->GetOwningLocalPlayer() == nullptr )
		return &SlateBrush;

	if (Icons.Contains(TagOrId))
	{
		return &Icons[TagOrId];
	}
	FString ContextString;

	FDataTableRowHandle RowData;
	RowData.DataTable = CommonInputActionDataTable;
	RowData.RowName = TagOrId;

	TArray<FDataTableRowHandle> Handle;
	Handle.Add(RowData);

	if (UCommonInputSubsystem* CommonInput = GetInputSubsystem())
	{
		const FSlateBrush GetIcon = CommonUI::GetIconForInputActions(CommonInput, Handle);
		Icons.Add(TagOrId, GetIcon);

		return &Icons[TagOrId];
	}

	return &SlateBrush;
}

UCommonInputSubsystem* UCommonLabInputActionDecorator::GetInputSubsystem() const
{
	if (OwnerRichTextBlock != nullptr)
	{
		return UCommonInputSubsystem::Get(OwnerRichTextBlock->GetOwningLocalPlayer());
	}

	return nullptr;
}

FVector2D UCommonLabInputActionDecorator::GetIconLocation(int32 FontSize)
{
	if(CommonActionIconLocationByFontSize.Contains(FontSize))
		return CommonActionIconLocationByFontSize[FontSize];
	
	return CommonActionIconLocation;
}

void UCommonLabInputActionDecorator::OnInputMethodChanged(ECommonInputType InputType)
{
	// 이미지 맵 비우고
	Icons.Empty();

	// RichTextBlock에 있는 모든 텍스트를 다시 그립니다.
	if (OwnerRichTextBlock.IsValid())
		OwnerRichTextBlock->RefreshTextLayout();
}
