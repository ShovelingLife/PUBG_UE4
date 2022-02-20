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

UCLASS()
class UI_PUBG_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
    /** \brief 파티원 정보 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UMemberStateUI* MemberStateUI; 

    /** \brief 플레이어 현재 상태 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UPlayerStateUI* PlayerStateUI;
    
    /** \brief 사망자 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UKillLogBoxUI*    KillLogBoxUI;
    
    /** \brief 미니맵을 띄워주는 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UMiniMapUI*        MinimapUI;
    
    /** \brief 현재 착용 중인 무기 UI */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UCurrentWeaponUI* CurrentWeaponUI;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};