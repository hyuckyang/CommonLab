#include "CommonLabPreLoadingScreenBase.h"

#define LOCTEXT_NAMESPACE "CommonLabPreLoadingScreenBase"

void FCommonLabPreLoadingScreenBase::Init()
{
	if (!GIsEditor && FApp::CanEverRender())
	{
		//EngineLoadingWidget = SNew(SPrimaryPreLoadingScreenWidget);
	}
}

#undef LOCTEXT_NAMESPACE