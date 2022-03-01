#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameInstanceSubsystemUI.generated.h"

class AUI_manager;

//DECLARE_DELEGATE_OneParam(FDeleSetTooltipData,)
DECLARE_DELEGATE_OneParam(FDeleHideTooltip, ESlateVisibility)

UCLASS()
class UI_PUBG_API UGameInstanceSubsystemUI : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TSubclassOf< AUI_manager> UImanagerClass;

public:
    UPROPERTY() AUI_manager* pUImanager;
    FDeleHideTooltip DeleHideTooltip;

public:
	UGameInstanceSubsystemUI();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
    void InitUImanager();
};