// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonLabLoadingScreenManager.h"
#include "CommonLabLoadingShouldInterface.h"
#include "CommonLabLoadingScreenSetting.h"
#include "GameFramework/GameStateBase.h"
#include "Process/LoadingProcess.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UCommonLabLoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::HandlePreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::HandlePostLoadMap);

	const UGameInstance* LocalGameInstance = GetGameInstance();
	check(LocalGameInstance);
}

void UCommonLabLoadingScreenManager::Deinitialize()
{
	// Clean 
	if (Process)
	{
		Process->Clean();
		Process = nullptr;
	}
	
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

bool UCommonLabLoadingScreenManager::ShouldCreateSubsystem(UObject* Outer) const
{
	// Dedicate Server 가 아닌 경우에만 생성  
	const UGameInstance* GameInstance = CastChecked<UGameInstance>(Outer);
	const bool bIsDedicatedServer = GameInstance->IsDedicatedServerInstance();
	return !bIsDedicatedServer;
}

void UCommonLabLoadingScreenManager::Tick(float DeltaTime)
{
	// 로딩 스크린이 끝났다면 더이상 Tick 에서 Screen 필요성을 체크하지 않습니다.
	if (Process != nullptr && Process->FadeTick(DeltaTime))
		return;
	
	if (!bCurrentlyShowLoadScreen)
		return;

	UpdateLoadScreen();
}

ETickableTickType UCommonLabLoadingScreenManager::GetTickableTickType() const
{
	// IsTickable 의 조건에 따라 Tick 을 활성화 합니다.
	return ETickableTickType::Conditional; 
}

bool UCommonLabLoadingScreenManager::IsTickable() const
{
	// CDO 가 아닌 생성된 객체 일 경우에만 Tick 을 활성화 합니다.
	return !HasAnyFlags(RF_ClassDefaultObject);
}

TStatId UCommonLabLoadingScreenManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCommonLabLoadingScreenManager, STATGROUP_Tickables); 
}

UWorld* UCommonLabLoadingScreenManager::GetTickableGameObjectWorld() const
{
	// GameInstance 가 일관된 World 를 반환합니다.
	return GetGameInstance()->GetWorld();
}

void UCommonLabLoadingScreenManager::OnFade(bool bFadeOut, float Duration, FLinearColor Color)
{
	FLinearColor ElapsedColor = FLinearColor::White;
	float ElapsedTime = Duration;
	bool bShouldConnectFade = false;
	bool bFadeAway = Process != nullptr ? Process->GetIsFadeAway() : false;
	
	if (!HandleProcessElapsed(Duration, ElapsedColor, ElapsedTime, bShouldConnectFade))
		return;

	Process = NewObject<UFadeProcess>(this);
	if (bShouldConnectFade && bFadeAway)
	{
		Process->FadeFunc(bFadeOut, ElapsedTime, ElapsedColor, Color);
	}
	else
	{
		Process->FadeFunc(bFadeOut, Duration, Color);
	}
}

void UCommonLabLoadingScreenManager::OnLoadLevel(FName LoadLevel, float Duration, FLinearColor Color)
{
	TSubclassOf<UUserWidget> Subclass = nullptr;
	if (const UCommonLabLoadingScreenSetting* Setting = GetDefault<UCommonLabLoadingScreenSetting>())
	{
		Subclass = Setting->DefaultWidgetClass.TryLoadClass<UUserWidget>();
	}

	OnLoadLevelBySubClass(LoadLevel, Duration, Subclass, Color);
}

void UCommonLabLoadingScreenManager::OnLoadLevelBySubClass(FName LoadLevel, float Duration, TSubclassOf<class UUserWidget> LoadingSubClass, FLinearColor Color)
{
	FLinearColor ElapsedColor = FLinearColor::White;
	float ElapsedTime = 0.f;
	bool bShouldConnectFade = false;
	bool bFadeAway = Process != nullptr ? Process->GetIsFadeAway() : false;
	
	if (!HandleProcessElapsed(Duration, ElapsedColor, ElapsedTime, bShouldConnectFade))
		return;

	// 로딩 스크린 시작
	bCurrentlyShowLoadScreen = true; 
	ULoadingProcess* LoadProcess = NewObject<ULoadingProcess>(this);
	if (bShouldConnectFade && bFadeAway)
	{
		LoadProcess->LoadStart(ElapsedTime, LoadingSubClass, ElapsedColor, Color, TDelegate<void()>::CreateLambda([this, LoadLevel]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), LoadLevel);
		}));
	}
	else
	{
		LoadProcess->LoadStart(Duration, LoadingSubClass, Color, TDelegate<void()>::CreateLambda([this, LoadLevel]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), LoadLevel);
		}));
	}
	
	Process = LoadProcess;
}

bool UCommonLabLoadingScreenManager::HandleProcessElapsed(float Duration, FLinearColor& OutElapsedColor, float& OutElapsedTime, bool& OutShouldConnectFade)
{
	if (Process)
	{
		if (ULoadingProcess* LoadingProcess = Cast<ULoadingProcess>(Process))
		{
			// Fade 가 아닌 로딩 중이라고 한다면, Fade 를 진행하지 않습니다.
			if (LoadingProcess->IsLoadProcess())
				return false;
		}

		OutElapsedColor = Process->GetElapsedColor();
		OutElapsedTime = Duration - (Duration * Process->GetElapsedRate());

		OutShouldConnectFade = true;

		Process->Clean();
		Process = nullptr;
	}

	return true;
}

void UCommonLabLoadingScreenManager::RegisterShouldInterface(TScriptInterface<ICommonLabLoadingShouldInterface> Interface)
{
	if (ShouldLoadingProcess.Contains(Interface.GetObject()) == false)
		ShouldLoadingProcess.Add(Interface.GetObject());
}

void UCommonLabLoadingScreenManager::UnRegisterShouldInterface(TScriptInterface<ICommonLabLoadingShouldInterface> Interface)
{
	if (ShouldLoadingProcess.Contains(Interface.GetObject()))
		ShouldLoadingProcess.Remove(Interface.GetObject());
}

void UCommonLabLoadingScreenManager::HandlePreLoadMap(const FWorldContext& Context, const FString& MapName)
{
	if (Context.OwningGameInstance == GetGameInstance())
	{
		bCurrentlyInLoadMap = true;

		// Loading 도중, 기본 Throbber 가 출력되는 것을 예외처리 
		GEngine->RegisterBeginStreamingPauseRenderingDelegate(nullptr);
		GEngine->RegisterEndStreamingPauseRenderingDelegate(nullptr);
	}
}

void UCommonLabLoadingScreenManager::HandlePostLoadMap(UWorld* World)
{
	if((World != nullptr) && World->GetGameInstance() == GetGameInstance())
	{
		bCurrentlyInLoadMap = false;
	}
}

bool UCommonLabLoadingScreenManager::IsShouldShowLoadScreen() const
{
	const UGameInstance* GameInstance = GetGameInstance();

	const FWorldContext* Context = GameInstance->GetWorldContext();
	if(Context == nullptr)
		return true;

	UWorld* World = Context->World();
	if(World == nullptr)
		return true;

	AGameStateBase* GameState = World->GetGameState<AGameStateBase>();
	if(GameState == nullptr)
		return true;

	// 현재 레벨이 로딩 중이라면
	if(bCurrentlyInLoadMap)
		return true;

	// 현재 이동이 보류 된? Level 이 있다면 계속 로딩창 이 출력
	if(!Context->TravelURL.IsEmpty())
		return true;

	// Context->PendingNetGame 은 일단 체크하지 않습니다.

	// 아직 World의 Begin Play 가 실행되지 않았다면 계속 대기 합니다.
	if(!World->HasBegunPlay())
		return true;

	// 현재 World 가 계속 이동중이라면 대기 
	if(World->IsInSeamlessTravel())
		return true;

	// Should -> 개발 테스트 중 
	{
		//State 자체에 인터페이스가 존재한다면.
		if(ICommonLabLoadingShouldInterface::ShouldShowLoadingScreen(GameState))
		{
			return true;
		}
		
		for(UActorComponent* TestComponent : GameState->GetComponents())
		{
			if (ICommonLabLoadingShouldInterface::ShouldShowLoadingScreen(TestComponent))
			{
				return true;
			}
		}
		
		for (TWeakInterfacePtr ShouldInterface : ShouldLoadingProcess)
		{
			if (ICommonLabLoadingShouldInterface::ShouldShowLoadingScreen(ShouldInterface.GetObject()))
				return true;
		}
	}

#pragma region 플레이어가 n명 이상 존재시 필요한 로직
	// 화면 분활 에 따른 로딩
	// 아래 내용 중 주석 된 내용은 추후 삭제해도 큰 문제가 없습니다.
	bool bFoundAnyLocalPC = false;
	bool bMissingAnyLocalPC = false;

	for (ULocalPlayer* LPlayer : GameInstance->GetLocalPlayers())
	{
		if(LPlayer != nullptr)
		{
			if(APlayerController* PController = LPlayer->PlayerController)
			{
				// 로컬 플레이어 중 1명 이라도 컨트롤러가 존재한다면
				bFoundAnyLocalPC = true;

				// 플레이어 컨트롤러 에도 ICommonLabLoadingShouldInterface 있다면 검사, Should -> 개발 테스트 중 
				if (ICommonLabLoadingShouldInterface::ShouldShowLoadingScreen(PController))
					return true;
			}
			else
			{
				// 로컬 플레이어들 중 1명 이라도 컨트롤러가 없다면
				bMissingAnyLocalPC = false;
			}
		}
	}

	// Split -> 화면 분활
	UGameViewportClient* GameViewportClient = GameInstance->GetGameViewportClient();
	if(GameViewportClient->GetCurrentSplitscreenConfiguration() != ESplitScreenType::None)
	{
		// 화면 분활 되었으며 로컬 플레이어 중 1명 이라도 컨트롤러가 없다면
		if(bMissingAnyLocalPC)
			return true;
	}
	else
	{
		// 화면은 1개 이나 플레이어 컨트롤러가 1개 도 없다면
		if(!bFoundAnyLocalPC)
			return true;
	}
#pragma endregion
	
	return false;
}

void UCommonLabLoadingScreenManager::UpdateLoadScreen()
{
	if(IsShouldShowLoadScreen())
	{
		// 필요시 로직을 추가 합니다.
	}
	else
	{
		HideLoadScreen();
	}
}

void UCommonLabLoadingScreenManager::HideLoadScreen()
{
	// 이미 로딩 스크린이 종료되었다면
	if (!bCurrentlyShowLoadScreen)
		return;

	//
	// 추가 로직 
	//
	
	bCurrentlyShowLoadScreen = false;
	
	if (Process)
	{
		if (ULoadingProcess* LoadingProcess = Cast<ULoadingProcess>(Process))
			LoadingProcess->LoadEnd();
	}
}
