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
DECLARE_DELEGATE_OneParam(FDeleSetInventoryCapacity, int)
DECLARE_DELEGATE_OneParam(FDeleDeleteInventoryItem, FString)
DECLARE_DELEGATE_OneParam(FDeleSetFadingTxt, FString)
DECLARE_DELEGATE_OneParam(FDeleSetExplosiveUI, ACoreThrowableWeapon*)
DECLARE_DELEGATE_RetVal_OneParam(int, FDeleGetBulletCount, FString)
DECLARE_DELEGATE_TwoParams(FDeleUpdateWorldList, ABaseInteraction*, bool)
DECLARE_DELEGATE_TwoParams(FDeleSetItemOntoInventory, ABaseInteraction*, bool)
DECLARE_DELEGATE_RetVal_TwoParams(bool, FDeleSetInteractionWidgetComp, UWidgetComponent*, FString)
DECLARE_DELEGATE_ThreeParams(FDeleRunEffectAnim, float, float, EPlayerStateAnimType)

// 무기 관련 델리게이트
DECLARE_DELEGATE(FDelePredictGrenadePath)

/** \brief 게임 인스턴스 클래스 */
UCLASS()
class PUBG_UE4_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
    // 싱글톤
    static UCustomGameInstance* inst;

public:
    /** \brief 전역 변수 */
    UPROPERTY() ASoundManager* pSoundManager;
    FDeleKillUI_Anim DeleKillUI_Anim;
    FDeleSetPlayerOtherState      DeleSetPlayerOtherState;
    FDeleDealPlayerDmg            DeleDealPlayerDmg;
    FDeleSetInventoryCapacity     DeleSetInventoryCapacity;
    FDeleDeleteInventoryItem      DeleDeleteInventoryItem;
    FDeleSetFadingTxt             DeleSetFadingTxt;
    FDeleSetExplosiveUI           DeleSetExplosiveUI;
    FDeleGetBulletCount           DeleGetBulletCount;
    FDeleUpdateWorldList          DeleUpdateWorldList;
    FDeleSetItemOntoInventory     DeleSetItemOntoInventory;
    FDeleSetInteractionWidgetComp DeleSetInteractionWidgetComp;
    FDeleRunEffectAnim            DeleRunEffectAnim;

    FDelePredictGrenadePath DelePredictGrenadePath;

public:
    // 싱글톤 가져오기
    static UCustomGameInstance* GetInst() { return inst; }

public:
    UCustomGameInstance();

protected:
    virtual void OnStart() override;
};