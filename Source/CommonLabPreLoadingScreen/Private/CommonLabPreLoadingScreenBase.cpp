#include "CommonLabPreLoadingScreenBase.h"
#include "CommonLabPreLoadingScreenWidget.h"

#define LOCTEXT_NAMESPACE "CommonLabPreLoadingScreenBase"

void FCommonLabPreLoadingScreenBase::Init()
{
	if (!GIsEditor && FApp::CanEverRender())
	{
		EngineLoadingWidget = SNew(SCommonLabPreLoadingScreenWidget);
	}
}

#undef LOCTEXT_NAMESPACE