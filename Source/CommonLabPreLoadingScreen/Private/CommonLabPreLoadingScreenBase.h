#pragma once

#include "PreLoadScreenBase.h"

class SWidget;

class FCommonLabPreLoadingScreenBase : public FPreLoadScreenBase
{
public:

	// Begin IPreLoadScreen Implementation
	virtual void Init() override;
	virtual EPreLoadScreenTypes GetPreLoadScreenType() const override { return EPreLoadScreenTypes::EngineLoadingScreen; }
	virtual TSharedPtr<SWidget> GetWidget() override { return EngineLoadingWidget; }
	// End IPreLoadScreen Implementation
	
private:
	
	TSharedPtr<SWidget> EngineLoadingWidget;
};
