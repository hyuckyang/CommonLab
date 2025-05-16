#include "CommonLabPreLoadingScreenWidget.h"

#include "CommonLabPreLoadingScreen.h"
#include "CommonLabPreLoadingScreenSetting.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Engine/UserInterfaceSettings.h"
#include "Slate/DeferredCleanupSlateBrush.h"

void SCommonLabPreLoadingScreenWidget::Construct(const FArguments& InArgs)
{
	const UCommonLabPreLoadingScreenSetting* LoadingScreenSettings = GetDefault<UCommonLabPreLoadingScreenSetting>();

	TSharedRef<SOverlay> Root = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill);

	Root->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SDPIScaler)
			.DPIScale(this, &SCommonLabPreLoadingScreenWidget::GetDPIScale)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderBackgroundColor(FLinearColor::Black)
			]
		];
	
	if (FCommonLabPreLoadingScreenModule::s_LoadedPreScreenImage)
	{
		Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image_Lambda([]()
					{
						return FCommonLabPreLoadingScreenModule::s_LoadedPreScreenImage->GetSlateBrush();
					})
			.ColorAndOpacity_Lambda([this]()
				{
					return FSlateColor(FLinearColor(1, 1, 1, 1.0f));
				})
			];
	}

	// 현재 테스트 중에 있습니다.
	// if (FCommonLabPreLoadingScreenModule::s_LoadedPreCircleMaterial)
	// {
	// 	Root->AddSlot()
	// 		.HAlign(HAlign_Center)
	// 		.VAlign(VAlign_Top)
	// 	[
	// 		SNew(SBox)
	// 		.WidthOverride(128.0f)
	// 		.HeightOverride(128.0f)
	// 		[
	// 			SNew(SImage)
	// 			.Image(FCommonLabPreLoadingScreenModule::s_LoadedPreCircleMaterial.Get())
	// 			.RenderTransformPivot(FVector2D(0.5f, 0.5f))
	// 			.RenderTransform(FSlateRenderTransform( FScale2D(1.f,1.f), FVector2D(0.f,0.f)))
	// 			.ColorAndOpacity_Lambda([this]()
	// 			{
	// 				return FSlateColor(FLinearColor(1, 1, 1, 1.0f));
	// 			})
	// 		]
	// 	];
	// }

	if (LoadingScreenSettings->bIsPreLoadingCircleThrobber)
	{
		TSharedRef<SCircularThrobber> Circular = SNew(SCircularThrobber)
		.PieceImage(&LoadingScreenSettings->PreLoadingCircleThrobberImage)
		.NumPieces(LoadingScreenSettings->PreLoadingCircleNumberOfPieces)
		.Period(LoadingScreenSettings->PreLoadingCirclePeriod)
		.Radius(LoadingScreenSettings->PreLoadingCircleRadius);
	
		Circular->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
		Circular->SetRenderTransform(FSlateRenderTransform( FScale2D(1.f,1.f)));
		Root->AddSlot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
		[
			Circular
		];
	}
	
	ChildSlot
	[
		Root
	];
}

float SCommonLabPreLoadingScreenWidget::GetDPIScale() const
{
	const FVector2D& DrawSize = GetCachedGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size(static_cast<int32>(DrawSize.X), static_cast<int32>(DrawSize.Y));
	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

#undef LOCTEXT_NAMESPACE
