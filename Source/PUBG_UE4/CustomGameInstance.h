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
#include "MyEnum.h"
#include "CustomGameInstance.generated.h"

class UWidgetComponent;
class ASoundManager;
class ABaseInteraction;
class ACoreThrowableWeapon;

DECLARE_DELEGATE_OneParam(FDeleSetPlayerOtherState, EPlayerOtherState)

// UI 관련 델리게이트
DECLARE_DELEGATE(FDeleKillUI_Anim)
DECLARE_DELEGATE_OneParam(FDeleDealPlayerDmg, float)
DECLARE_DELEGATE_OneParam(FDeleSetShootTypeNotificationTxt, FString)
DECLARE_DELEGATE_TwoParams(FDeleSetItemOntoInventory, ABaseInteraction*, bool)
DECLARE_DELEGATE_TwoParams(FDeleUpdateInteractionWidgetComp, UWidgetComponent*, FString)
DECLARE_DELEGATE_ThreeParams(FDeleRunEffectAnim, float, float, EPlayerStateAnimType)

/** \brief 게임 인스턴스 클래스 */
UCLASS()
class PUBG_UE4_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    /** \brief 전역 변수 */
    UPROPERTY() ASoundManager*       pSoundManager;
    FDeleSetPlayerOtherState DeleSetPlayerOtherState;
    FDeleDealPlayerDmg               DeleDealPlayerDmg;
    FDeleSetShootTypeNotificationTxt DeleSetShootTypeNotificationTxt;
    FDeleSetItemOntoInventory        DeleSetItemOntoInventory;
    FDeleUpdateInteractionWidgetComp DeleUpdateInteractionWidgetComp;
    FDeleRunEffectAnim               DeleRunEffectAnim;
    FDeleKillUI_Anim DeleKillUI_Anim;

public:
    UCustomGameInstance();

protected:
    virtual void OnStart() override;
};