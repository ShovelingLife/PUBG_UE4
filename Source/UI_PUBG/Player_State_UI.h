/**
 * \file Player_State_UI.h
 *
 * \brief 현재 플레이어 상태 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_State_UI.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class AUI_manager;

UCLASS()
class UI_PUBG_API UPlayer_State_UI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /** \brief UI 매니저 */
    UPROPERTY() AUI_manager* mp_UI_manager;

public:
    /**
      * \brief 체력 회복력(에너지드링크, 진통제바) 산소
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* HP_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* Recovery_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* Oxygen_bar;

    /**
      * \brief 장비 착용 여부 이미지 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Backbag_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Head_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Bodyarmor_img;

    /**
      * \brief 버프 지속 여부 이미지 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Speedup_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Healing_img;

    /**
      * \brief 사용중인 총기 현황
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Magazine_current_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Magazine_total_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     BoltAction_img;

    /** \brief 자세 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Posture_img;

    /** \brief 조준선 이미지 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Reticle_img;

    class ACustom_player* p_player;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;

public:
    /**
      * \brief 조준선 갱신 
     */
    void Update_aim_UI();

    /**
      * \brief 발사 방법 UI 갱신
     */
    void Update_shoot_mode();

    /**
      * \brief 총알 개수 UI 갱신
     */
    void Update_bullet_count_UI();

    /**
      * \brief 산소 바 UI 갱신
     */
    void Update_oxygen_bar_UI(float);
};