#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player_State_UI.generated.h"

UCLASS()
class UI_PUBG_API UPlayer_State_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 체력 회복력(에너지드링크, 진통제바) 산소
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* HP_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Recovery_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Oxygen_bar;

    // 장비 착용 여부 이미지 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Backbag_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Head_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Bodyarmor_img;

    // 버프 지속 여부 이미지 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Speedup_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Healing_img;

    // 사용중인 총기 현황
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_current_text;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_total_text;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* BoltAction_img;

    // 자세
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Posture_img;

    // 조준선 이미지
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Reticle_img;

    class ACustom_player* p_custom_player;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;

public:
    // 무기 슬롯 갱신
    void Update_weapon_slot_UI();

    // 조준선 갱신
    void Update_aim_UI();

    // 총알 개수 갱신
    void Update_bullet_count_UI();

    // 산소 바 갱신
    void Update_oxygen_bar_UI(float);

    void Change_shoot_mode();
};
