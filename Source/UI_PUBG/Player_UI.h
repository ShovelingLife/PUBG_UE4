/**
 * \file Player_UI.h
 *
 * \brief 메인 플레이어 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_UI.generated.h"

UCLASS()
class UI_PUBG_API UPlayer_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    /** \brief 파티원 정보 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UMember_State_UI*   Member_state_UI; 

    /** \brief 플레이어 현재 상태 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UPlayer_State_UI*   Player_state_UI;
    
    /** \brief 사망자 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UKillLog_Box_UI*    Kill_log_box_UI;
    
    /** \brief 미니맵을 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UMiniMap_UI*        Minimap_UI;
    
    /** \brief 현재 착용 중인 무기 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UCurrent_Weapon_UI* Current_weapon_UI;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};