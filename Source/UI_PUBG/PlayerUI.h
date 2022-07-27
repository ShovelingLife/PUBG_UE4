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
#include "PlayerUI.generated.h"

class UImage;
class UMemberStateUI;
class UPlayerStateUI;
class UKillLogBoxUI;
class UMiniMapUI;
class UCurrentWeaponUI;
class UNotificationUI;

UCLASS()
class UI_PUBG_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

private:
    // 현재 쓰여지는 이미지 배열
    UPROPERTY(VisibleAnywhere) TArray<UImage*> mArrImage;

public:
    /** \brief 파티원 정보 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UMemberStateUI* MemberStateUI; 

    /** \brief 플레이어 현재 상태 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UPlayerStateUI* PlayerStateUI;
    
    /** \brief 사망자 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UKillLogBoxUI* KillLogBoxUI;
    
    /** \brief 미니맵을 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UMiniMapUI* MinimapUI;
    
    /** \brief 현재 착용 중인 무기 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCurrentWeaponUI* CurrentWeaponUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UNotificationUI* NotificationUI;

    /** \brief 조준선 이미지 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CrosshairUpImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CrosshairDownImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CrosshairLeftImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CrosshairRightImg;

    UPROPERTY(EditAnywhere) float TmpPosVal = 0.f;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

private:
    /** \brief 발사 방법 UI 갱신 */
    void UpdateAimUI();

    void UpdateCrossHair();

};