// Fill out your copyright notice in the Description page of Project Settings.

#include "Activatable/CommonLabActivatableSubClass.h"
#include "Activatable/CommonLabActivatableSettings.h"
//#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

void UCommonLabActivatableSubClass::Initialize()
{
	// // 게임이 초기화 되고, 그 초기화된 상태에서 위젯이 초기화 되겠금 유도 하였습니다.
	// // 즉 GameMode, GameInstance, LocalPlayer 가 초기화 된 상태에서 위젯이 초기화 되도록 합니다
	// GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
	// {
	// 	AddRootFromViewport();
	// 	AddActivatableClass();
	// }));

	// 호출 순서가 명확하기에 위의 1 Tick 을 대기하는 것은 일단 보류 합니다.
	AddRootFromViewport();
	AddActivatableClass();
}

void UCommonLabActivatableSubClass::Released()
{
	RemoveRootFromViewport();
}


void UCommonLabActivatableSubClass::Destroyed()
{
	DestroyRootFromViewport();
}

void UCommonLabActivatableSubClass::AddRootFromViewport()
{
	// Root 를 생성, 이미 있다면 해당 로직은 무시 됩니다.
	if (Root != nullptr || !LocalPlayer.IsValid())
		return;

	if (APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld()))
	{
		// Root 생성
		Root = Cast<UUserWidget>(CreateWidget<UCommonUserWidget>(GetWorld(), UCommonUserWidget::StaticClass(), FName(TEXT("CommonLab Activatable Root"))));
		Root->SetPlayerContext(FLocalPlayerContext(Cast<ULocalPlayer>(LocalPlayer.Get())));

		// UCommonUserWidget::StaticClass 으로 생성 시, WidgetTree 혹은 (블루프린트로) WidgetTree 의 루트 위젯이 없으며 이때 AddToPlayerScreen 를 시도시,
		// 제대로 된 UMG 루트 내에 생성이 안될 수 있습니다. ( 실제로 안됩니다. )
		// Root 생성 직후  WidgetTree 가 없다면 WidgetTree 를 생성
		// 그리고 RootOverlay 를 설정 합니다.
		if (Root->WidgetTree == nullptr)
			Root->WidgetTree = NewObject<UWidgetTree>();

		RootOverlay = Root->WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Main_Overlay"));
		Root->WidgetTree->RootWidget = RootOverlay;

		Root->AddToPlayerScreen(1000); // 기본 레이어

#if WITH_EDITOR
		if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
		{
			// Lyra 샘플 참조
			// So our controller will work in PIE without needing to click in the viewport
			// 첫번째 플레이어일 때는 무조건 뷰포트 0에 포커스를 맞추도록 합니다.
			FSlateApplication::Get().SetUserFocusToGameViewport(0);
		}
#endif

		const UCommonLabActivatableSettings* Setting = GetDefault<UCommonLabActivatableSettings>();
		
		for (int32 i = 0; i < Setting->ActivatableStackTags.Num(); i++)
		{
			FGameplayTag CurrTag = Setting->ActivatableStackTags[i];
			FGameplayTag PrevTag = FGameplayTag::EmptyTag;
			if (i > 0)
			{
				PrevTag = Setting->ActivatableStackTags[i - 1];
			}
			
			UCommonLabActivatableStackable* PrevStackable = nullptr;
			if (PrevTag != FGameplayTag::EmptyTag)
			{
				if (TWeakObjectPtr<UCommonLabActivatableStackable> Stackable = ActivatableStacks.FindRef(PrevTag); Stackable.IsValid())
				{
					PrevStackable = Stackable.Get();
				}
			}

			if (UCommonLabActivatableStackable* CurrStackable = AddStackable(CurrTag))
			{
				CurrStackable->SetPrevStackable(PrevStackable);
			}
		}
	}
}

void UCommonLabActivatableSubClass::RemoveRootFromViewport()
{
	if (Root == nullptr)
		return;

	// 캐싱된 위젯을 가져 옵니다.
	TWeakPtr<SWidget> LayoutSlateWidget = Root->GetCachedWidget();
	if(LayoutSlateWidget.IsValid())
	{
		// Cache 위젯이 존재한다면 Remove 합니다.
		Root->RemoveFromParent();
	}	
}

void UCommonLabActivatableSubClass::DestroyRootFromViewport()
{
	RemoveRootFromViewport();
	
	RootOverlay = nullptr;
	Root = nullptr;
}

UCommonLabActivatableStackable* UCommonLabActivatableSubClass::AddStackable(FGameplayTag Tag)
{
	UCommonLabActivatableStackable* LayerStack = NewObject<UCommonLabActivatableStackable>(Root->WidgetTree, Tag.GetTagName());
	LayerStack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	UOverlaySlot* OverlaySlot = RootOverlay->AddChildToOverlay(LayerStack);
	OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
	OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	OverlaySlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));

	// 이동 시, 
	// LayerStack->OnTransitioningChanged.AddUObject(this, &UPrimaryGameLayout::OnWidgetStackTransitioning);
	LayerStack->SetTransitionDuration(0.0);

	ActivatableStacks.Emplace(Tag, LayerStack);

	return ActivatableStacks[Tag].Get();
}

void UCommonLabActivatableSubClass::AddActivatableClass()
{
	const UCommonLabActivatableSettings* Setting = GetDefault<UCommonLabActivatableSettings>();
	const UWorld* World = GetWorld();
	const FString LevelName = World->GetName();
	FGameplayTag Tag = FGameplayTag::EmptyTag;
	FSoftClassPath ActivatableClass;
	
	if (Setting->LevelActivatableStructs.Num() > 0)
	{
		for( int32 i = 0; i < Setting->LevelActivatableStructs.Num(); ++i)
		{
			auto LevelData = Setting->LevelActivatableStructs[i];
			if (LevelData.Level.IsNull())
				continue;
			
			const FString ActivatableLevelName = LevelData.Level.GetAssetName();
			if (ActivatableLevelName == LevelName)
			{
				Tag = LevelData.ActivatableStruct.ActivatableTag;
				ActivatableClass = LevelData.ActivatableStruct.ActivatableWidget;

				Open(Tag, ActivatableClass.TryLoadClass<UCommonLabActivatableWidget>());
			}
		}
	}

	// 위의 로직(레벨별로 별도의 UI 를 출력하는 로직) 에서 Tag 가 설정되지 않았다면
	if (Tag == FGameplayTag::EmptyTag)
	{
		Tag = Setting->BasedActivatable.ActivatableTag;
		ActivatableClass = Setting->BasedActivatable.ActivatableWidget;

		Open(Tag, ActivatableClass.TryLoadClass<UCommonLabActivatableWidget>());	
	}
}


