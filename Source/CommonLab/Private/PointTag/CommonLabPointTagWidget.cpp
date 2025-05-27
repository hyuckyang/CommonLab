// Copyright Evr studio, Inc. All Rights Reserved.

#include "PointTag/CommonLabPointTagWidget.h"

#include "CommonLabSubsystem.h"
#include "PointTag/CommonLabPointTagSubClass.h"

void UCommonLabPointTagWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Deallocate();
	
	Super::ReleaseSlateResources(bReleaseChildren);
	
}

TSharedRef<SWidget> UCommonLabPointTagWidget::RebuildWidget()
{
	// 위젯이 디자인 타임(에디터에서 레이아웃을 편집하지 않는 상황)이 아니고 확장 태그가 유효한 경우 
	if (!IsDesignTime())
	{
		// 초기화 및, 등록 합니다.
		Deallocate();
		Allocate();
		
		// 
	}
	return Super::RebuildWidget();
}

void UCommonLabPointTagWidget::Allocate()
{
	if (GetGameInstance() != nullptr)
	{
		if (ULocalPlayer* LPlayer = GetOwningLocalPlayer())
		{
			if (UCommonLabSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UCommonLabSubsystem>())
			{
				if (UCommonLabPointTagSubClass* PointTagSubClass = Subsystem->GetSubClass<UCommonLabPointTagSubClass>(LPlayer))
				{
					Allocate(PointTagSubClass);
				}
			}	
		}
	}
}

void UCommonLabPointTagWidget::Deallocate()
{
	// Mapping 초기화
	// Handler 초기화
	// 테스트 중
	if(!IsDesignTime())
	{
		if (GetGameInstance() != nullptr)
		{
			if (ULocalPlayer* LPlayer = GetOwningLocalPlayer())
			{
				if (UCommonLabSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UCommonLabSubsystem>())
				{
					if (UCommonLabPointTagSubClass* PointTagSubClass = Subsystem->GetSubClass<UCommonLabPointTagSubClass>(LPlayer))
					{
						Deallocate(PointTagSubClass);
					}
				}	
			}
		}
	}
	
}

void UCommonLabPointTagWidget::Allocate(UCommonLabPointTagSubClass* TagSubClass)
{
	TagSubClass->Allocate<UObject*, bool>(PointTag, this, TEXT("PushPopVisible"))->AddFunction(this, &UCommonLabPointTagWidget::OnPushToVisible);
}

void UCommonLabPointTagWidget::Deallocate(UCommonLabPointTagSubClass* TagSubClass)
{
	TagSubClass->Deallocate(PointTag, this, TEXT("PushPopVisible"));

	// 초기화 
	PushToDisableObjects.Reset();
}

void UCommonLabPointTagWidget::OnPushToVisible(UObject* Object, bool bIsVisible)
{
	auto UpdateDisable = [this]()
	{
		bool bEnable = true;
		for(const FDisableState& State : PushToDisableObjects)
		{
			if(State.bIsDisable)
			{
				bEnable = false;
				break;
			}
		}
		
		if (OnPointWidgetVisible.IsBound())
		{
			OnPointWidgetVisible.Broadcast(this, bEnable);
		}
	};

	PushToDisableObjects.RemoveAll([](const FDisableState& State)
		{
			return !State.TargetObject.IsValid();
		});
	
	// 키를 다시 정렬합니다.
	for (int i = 0; i < PushToDisableObjects.Num(); ++i)
	{
		PushToDisableObjects[i].Key = i;
	}
	
	bool bIsEmpty = PushToDisableObjects.IsEmpty();
	
	if(!bIsVisible)
	{
		// if(const FDisableState* DisableStat = PushToDisableObjects.FindByKey(Object))
		// {
		// 	if( DisableStat->bIsDisable == true)
		// 		return;
		// }

		const int Key = PushToDisableObjects.IsEmpty() ? 0 : PushToDisableObjects.Last().Key + 1;
		PushToDisableObjects.Add(FDisableState(Key, true, Object)); // Disable = true

		// 최초 Disable 상태일 때 업데이트 해줍니다.
		if (Key == 0)
			UpdateDisable();
	}
	else
	{
		if(FDisableState* DisableStat = PushToDisableObjects.FindByKey(Object))
		{
			int Handle = DisableStat->Key;
			for(int i = 0; i < PushToDisableObjects.Num(); i++)
			{
				if(PushToDisableObjects[i].Key == Handle)
				{
					PushToDisableObjects.RemoveAt(i);
					Handle = -1;
					break;
				}
			}

			if (Handle != -1)
				return;

			UpdateDisable();
		}
	}
	
	if (bWasEmpty != bIsEmpty)
	{
		UpdateDisable();
		bWasEmpty = bIsEmpty;
	}
}
