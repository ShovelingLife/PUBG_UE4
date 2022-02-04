#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_UI.generated.h"

UCLASS()
class UI_PUBG_API UPlayer_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UMember_State_UI*   Member_state_UI; 
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UPlayer_State_UI*   Player_state_UI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UKillLog_Box_UI*    Kill_log_box_UI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UMiniMap_UI*        Minimap_UI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UCurrent_Weapon_UI* Current_weapon_UI;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};