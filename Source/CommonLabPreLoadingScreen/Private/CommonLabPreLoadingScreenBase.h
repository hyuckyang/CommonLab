#pragma once

#include "PreLoadScreenBase.h"

class SWidget;

class FCommonLabPreLoadingScreenBase : public FPreLoadScreenBase
{
public:

	// Begin IPreLoadScreen Implementation
	// 최초 게임 실행 시, 출력되는 로딩 화면을 설정 
	virtual void Init() override;
	virtual EPreLoadScreenTypes GetPreLoadScreenType() const override { return EPreLoadScreenTypes::EngineLoadingScreen; }
	virtual TSharedPtr<SWidget> GetWidget() override { return EngineLoadingWidget; }
	// End IPreLoadScreen Implementation
	
private:
	
	TSharedPtr<SWidget> EngineLoadingWidget;
};
