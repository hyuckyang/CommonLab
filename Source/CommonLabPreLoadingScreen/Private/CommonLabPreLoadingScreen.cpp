#include "CommonLabPreLoadingScreen.h"
#include "CommonLabPreLoadingScreenBase.h"
#include "CommonLabPreLoadingScreenSetting.h"
#include "PreLoadScreenManager.h"
#include "Slate/DeferredCleanupSlateBrush.h"

#define LOCTEXT_NAMESPACE "FCommonLabPreLoadingScreenModule"

TSharedPtr<FDeferredCleanupSlateBrush> FCommonLabPreLoadingScreenModule::s_LoadedPreScreenImage;
TSharedPtr<FSlateMaterialBrush> FCommonLabPreLoadingScreenModule::s_LoadedPreCircleMaterial;

void FCommonLabPreLoadingScreenModule::StartupModule()
{
	if (!IsRunningDedicatedServer())
	{
		TryLoadPreScreenImage();

		PreLoadingScreen = MakeShared<FCommonLabPreLoadingScreenBase>();
		PreLoadingScreen->Init();
	
		if (!GIsEditor && FApp::CanEverRender() && FPreLoadScreenManager::Get())
		{
			FPreLoadScreenManager::Get()->RegisterPreLoadScreen(PreLoadingScreen);
			FPreLoadScreenManager::Get()->OnPreLoadScreenManagerCleanUp.AddRaw(this, &FCommonLabPreLoadingScreenModule::OnPreLoadScreenManagerCleanUp);
		}
	}
}

void FCommonLabPreLoadingScreenModule::ShutdownModule()
{
    
}

bool FCommonLabPreLoadingScreenModule::IsGameModule() const
{
	return true;
}

void FCommonLabPreLoadingScreenModule::OnPreLoadScreenManagerCleanUp()
{
	PreLoadingScreen.Reset();
	ShutdownModule();
}

void FCommonLabPreLoadingScreenModule::TryLoadPreScreenImage()
{
	const UCommonLabPreLoadingScreenSetting* LoadingScreenSettings = GetDefault<UCommonLabPreLoadingScreenSetting>();

	UTexture2D* LoadedObject = nullptr;
	if (!LoadingScreenSettings->PreLoadingImage.IsNull())
	{
		LoadedObject = static_cast<UTexture2D*>(LoadingScreenSettings->PreLoadingImage.TryLoad());
	}

	if (LoadedObject != nullptr)
	{
		LoadedObject->NeverStream = true;
		s_LoadedPreScreenImage = FDeferredCleanupSlateBrush::CreateBrush(LoadedObject);
	}

	// 아래 코드는 현재 테스트 중에 있습니다.
	// UMaterialInterface* MaterialObject = nullptr;
	// if (!LoadingScreenSettings->PreLoadingLoadIndicatorMaterial.IsNull())
	// {
	// 	MaterialObject = static_cast<UMaterialInterface*>(LoadingScreenSettings->PreLoadingLoadIndicatorMaterial.TryLoad());
	// }
	//
	// if(MaterialObject != nullptr)
	// {
	// 	UMaterialInstanceDynamic* Dynamic = UMaterialInstanceDynamic::Create(MaterialObject, nullptr);
	// 	Dynamic->AddToRoot();
	// 	
	// 	s_LoadedPreCircleMaterial = MakeShared<FSlateMaterialBrush>(FVector2D(128, 128));
	// 	s_LoadedPreCircleMaterial->SetMaterial(Dynamic);
	// 	s_LoadedPreCircleMaterial->SetResourceObject(Dynamic);
	// }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCommonLabPreLoadingScreenModule, CommonLabPreLoadingScreen)