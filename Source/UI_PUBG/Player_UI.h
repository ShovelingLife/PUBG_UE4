#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Member_State_UI.h"
#include "Player_State_UI.h"
#include "KillLog_Box_UI.h"
#include "MiniMap_UI.h"
#include "Current_Weapon_UI.h"
#include "Player_UI.generated.h"

UCLASS()
class UI_PUBG_API UPlayer_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    // 플레이어 파티창 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UMember_State_UI* Member_state_UI; 

    // 현재 플레이어 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UPlayer_State_UI* Player_state_UI;

    // 킬로그 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UKillLog_Box_UI* Kill_log_box_UI;

    // 미니맵 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UMiniMap_UI* Minimap_UI;

    // 현재 무기 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCurrent_Weapon_UI* Current_weapon_UI;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};