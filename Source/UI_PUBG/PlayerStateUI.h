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
#include "PlayerStateUI.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class AUI_manager;

UCLASS()
class UI_PUBG_API UPlayerStateUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /** \brief UI 매니저 */
    UPROPERTY() AUI_manager* mpUI_manager;

public:
    /**
      * \brief 체력 회복력(에너지드링크, 진통제바) 산소
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* HP_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* RecoveryBar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* OxygenBar;

    /**
      * \brief 장비 착용 여부 이미지 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* BackbagImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* HeadImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* BodyArmorImg;

    /**
      * \brief 버프 지속 여부 이미지 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* SpeedupImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* HealingImg;

    /**
      * \brief 사용중인 총기 현황
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* MagazineCurrentTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* MagazineTotalTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     BoltActionImg;

    /** \brief 자세 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* PostureImg;

    /** \brief 조준선 이미지 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* ReticleImg;

    class ACustomPlayer* p_player;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;

public:
    /**
      * \brief 조준선 갱신 
     */
    void UpdateAimUI();

    /**
      * \brief 발사 방법 UI 갱신
     */
    void UpdateShootMode();

    /**
      * \brief 총알 개수 UI 갱신
     */
    void UpdateBulletCountUI();

    /**
      * \brief 산소 바 UI 갱신
     */
    void UpdateOxygenBarUI(float);
};