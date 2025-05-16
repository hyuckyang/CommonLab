#pragma once

#include "Widgets/SCompoundWidget.h"
#include "UObject/GCObject.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class FDeferredCleanupSlateBrush;

class SCommonLabPreLoadingScreenWidget : public SCompoundWidget //, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SCommonLabPreLoadingScreenWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
private:
	float GetDPIScale() const;
	bool bIsInitEngine = false;
};
