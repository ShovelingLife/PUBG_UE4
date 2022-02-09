#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Custom_game_instance.generated.h"

class UWidgetComponent;

DECLARE_DELEGATE_TwoParams(FDele_update_interaction_widget_comp, UWidgetComponent*, FString)

UCLASS()
class PUBG_UE4_API UCustom_game_instance : public UGameInstance
{
	GENERATED_BODY()

public:
    UPROPERTY() class ASound_manager* p_sound_manager;
    FDele_update_interaction_widget_comp  dele_update_interaction_widget_comp;

public:
    UCustom_game_instance();

protected:
    virtual void OnStart() override;
};