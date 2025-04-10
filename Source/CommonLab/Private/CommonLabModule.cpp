#include "Modules/ModuleInterface.h"

/*
 * Implementation of the CommonLab module.
 */

class FCommonLabModule : public IModuleInterface
{
public:
	FCommonLabModule();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};

FCommonLabModule::FCommonLabModule()
{
}

void FCommonLabModule::StartupModule()
{
}

void FCommonLabModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FCommonLabModule, CommonLab)