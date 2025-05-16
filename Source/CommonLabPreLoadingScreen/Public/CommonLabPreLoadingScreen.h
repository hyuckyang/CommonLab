#pragma once

#include "CoreMinimal.h"
#include "SlateMaterialBrush.h"
#include "Modules/ModuleManager.h"

class FDeferredCleanupSlateBrush;
class FCommonLabPreLoadingScreenBase;

/*
 * Common Startup Loading Screen 에서 Preload 2D Texture 를 추가한 정도 Module
 */
class FCommonLabPreLoadingScreenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    virtual bool IsGameModule() const override;

    static TSharedPtr<FDeferredCleanupSlateBrush> s_LoadedPreScreenImage;
    static TSharedPtr<FSlateMaterialBrush> s_LoadedPreCircleMaterial;

private:
    void OnPreLoadScreenManagerCleanUp();
    void TryLoadPreScreenImage();

    TSharedPtr<FCommonLabPreLoadingScreenBase> PreLoadingScreen;
    
};
