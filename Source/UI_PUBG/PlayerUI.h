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

class UCurrentWeaponUI;

class UImage;
class UKillLogBoxUI;
class UMemberStateUI;
class UMiniMapUI;
class UNotificationUI;
class UPlayerStateUI;
class USizeBox;

UCLASS()
class UI_PUBG_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

private:
    // 현재 쓰여지는 이미지 배열
    UPROPERTY(VisibleAnywhere) TArray<UImage*> mArrImage;

    class ACustomPlayer* pPlayer = nullptr;

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
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox* SB_CrosshairTop;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox* SB_CrosshairBottom;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox* SB_CrosshairLeft;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox* SB_CrosshairRight;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CrosshairCenterImg;

    FTimerHandle TCrosshairHandle;
    UPROPERTY(EditAnywhere, category = "CrosshairVar") float TestVel   = 0.f;
    UPROPERTY(EditAnywhere, category = "CrosshairVar") float MaxVel    = 350.f;
    UPROPERTY(EditAnywhere, category = "CrosshairVar") float MaxOffset = 100.f;
    UPROPERTY(EditAnywhere, category = "CrosshairVar") float UpdateSec = 60.f;
    
private:
    void UpdateCrosshairVisibility();

public:
    virtual void SynchronizeProperties() override;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

public:
    void HandleCrosshairScale();

    void HandleCrosshairScale(float Vel);
};