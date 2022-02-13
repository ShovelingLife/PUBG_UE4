/**
 * \file Custom_game_instance.h
 *
 * \brief 게임 인스턴스 = 싱글톤 통용
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Custom_game_instance.generated.h"

class UWidgetComponent;

DECLARE_DELEGATE_TwoParams(FDele_update_interaction_widget_comp, UWidgetComponent*, FString)

/**
  * \brief 게임 인스턴스 클래스
 */
UCLASS()
class PUBG_UE4_API UCustom_game_instance : public UGameInstance
{
	GENERATED_BODY()

public:
    /**
      * \brief 전역 변수
     */
    UPROPERTY() class ASound_manager* p_sound_manager;

    FDele_update_interaction_widget_comp  dele_update_interaction_widget_comp;

public:
    UCustom_game_instance();

protected:
    virtual void OnStart() override;
};