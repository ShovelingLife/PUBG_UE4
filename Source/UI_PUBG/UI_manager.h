﻿/**
 * \file UI_manager.h
 *
 * \brief UI 매니저
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotItemData.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/WeaponData.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "UI_manager.generated.h"

class UPlayerUI;
class UPlayerEffectUI;
class UCursorSlotUI;
class UItemSlotUI;
class AInventoryManager;
class UTooltipUI;

UCLASS()
class UI_PUBG_API AUI_manager : public AActor
{
	GENERATED_BODY()

public:
    static TMap<int, UTexture*>  MapPlayerIcon;           // 플레이어 UI

    // 무기 UI 관련 (인벤토리)
    UPROPERTY(EditAnywhere) TArray< UTexture*> ArrMainWeaponIcon;
    static TMap<int, UMaterial*> MapMainWeaponIcon;       // 메인 무기 UI 벡터 이미지  
    static TMap<int, UTexture*>  MapWeaponIcon;           // 인벤토리 무기 UI (총기)   
    static TMap<int, UTexture*>  MapOtherWeaponIcon;      // 인벤토리 무기 UI (근접 및 투척류)
    static TMap<int, UTexture*>  MapWeaponAttachmentIcon; // 부속품 UI

    // 그 외 UI 관련
    static TMap<int, UTexture*>  MapAmmoBoxIcon; // 탄박스 UI
    static TMap<int, UTexture*>  MapUtilityIcon; // 그 외 상호작용 가능한 오브젝트 UI (가방, 회복아이템 등)


private:
    // ------- 현재 쓰이고있는 변수들 -------

    /** * \brief 플레이어 UI */
    TSubclassOf<UPlayerUI> mBP_PlayerUI;
    UPROPERTY() UPlayerUI* mpPlayer_UI;

    /** * \brief 플레이어 상태이상 UI */
    TSubclassOf<UPlayerEffectUI> mBP_PlayerEffectUI;

    /** * \brief 상호작용 UI 관련 */
    UPROPERTY(VisibleAnywhere, Category = WidgetBP) TSubclassOf<UUserWidget> mInteractionWidgetBP;

    /** * \brief 메인 플레이어 UI 리소스 경로명 */
    const TArray<FString> mkArrShootType 
    {
        "SingleShot",
        "BurstShot",
        "ConsecutiveShot",
    };

public:
    UPROPERTY() AInventoryManager* pInventoryManager;
    UPROPERTY() UPlayerEffectUI*   pPlayerEffect_UI;

public:
    AUI_manager();

public:
    static UTexture2D* GetTexture2D(FsSlotItemData ItemData);

    static UTexture2D* GetTexture2D(int Index, FString Type = "");

    static UMaterial* GetMaterial(int Index);

    static UCursorSlotUI* GetSlotCursorUI(FsSlotItemData Data);

protected:
    virtual void BeginPlay() override;

private:
    // ------- 초기화 관련 -------

    void BindDelegate();

    /** \brief 상호작용 UI 초기화 */
    void InitInteractionUI();

    /** \brief 최상위 플레이어 UI 초기화 */
    void InitPlayerUI();

    /** \brief 플레이어 상태이상 UI 초기화 */
    void InitPlayerEffectUI();

    // ------- 아이콘 UI 관련 -------

    /** \brief 플레이어 UI 초기화 */
    void InitPlayerIcon();

    /** \brief 플레이어 무기 선택 UI 초기화 */
    void InitMainWeaponIcon();

    /** \brief 인벤토리 무기 UI 초기화 */
    void InitInventoryWeaponIcon();

    /** \brief 기타 UI 초기화 */
    void InitUtilityIcon();
    
// 시작 후 초기화
public:
    /** \brief 플레이어 인벤토리 설정 */
    void InitPlayerInventory();

    /** \brief 플레이어 UI 띄움 */
    void SetPlayerUI();

    /** \brief 미구현 상태 */
    void SetWeaponUI();

public:
    /** \brief 상호작용 UI 업데이트 (델리게이트) */
    UFUNCTION() void UpdateInteractionUI(class UWidgetComponent* WidgetComp, FString Title);

    UFUNCTION() void RunEffectAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type);

    UFUNCTION() void KillAnim();
};