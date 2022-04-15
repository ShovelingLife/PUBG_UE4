/**
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
class UItemSlotUI;
class AInventoryManager;
class UTooltipUI;

UCLASS()
class UI_PUBG_API AUI_manager : public AActor
{
	GENERATED_BODY()

private:
    // ------- 현재 쓰이고있는 변수들 -------

    /** * \brief 플레이어 UI */
    TSubclassOf<UPlayerUI> mPlayerUI_BP;
    UPROPERTY() UPlayerUI* mpPlayer_UI;

    /** * \brief 플레이어 상태이상 UI */
    TSubclassOf<UPlayerEffectUI> mPlayerEffectUI_BP;

    /** * \brief 상호작용 UI 관련 */
    UPROPERTY(VisibleAnywhere, Category = WidgetBP) TSubclassOf<UUserWidget> mInteractionWidgetBP;

    /** * \brief 메인 플레이어 UI 리소스 경로명 */
    const TArray<FString> mkArrPlayerUI_TexPath 
    {
        "SingleShot",
        "BurstShot",
        "ConsecutiveShot",
    };

public:
    static TMap<int, UMaterial*> MapMainWeaponMat;      // 메인 무기 UI    
    static TMap<int, UTexture*>  MapInventoryGunWeaponTex; // 인벤토리 무기 UI (총기)   
    static TMap<int, UTexture*>  MapInventoryOtherWeaponTex; // 인벤토리 무기 UI (근접 및 투척류)
    static TMap<int, UTexture*>  MapPlayerTex;          // 플레이어 UI

    UPROPERTY() AInventoryManager* pInventoryManager;
    UPROPERTY() UPlayerEffectUI*   pPlayerEffect_UI;

public:
    AUI_manager();

protected:
    virtual void BeginPlay() override;

// 생성자 내 초기화 함수
private:
    /** \brief 상호작용 UI 초기화 */
    void InitInteractionUI();

    /** \brief 최상위 플레이어 UI 초기화 */
    void InitPlayerUI();

    /** \brief 플레이어 상태이상 UI 초기화 */
    void InitPlayerEffectUI();

    /** \brief 플레이어 UI 초기화 */
    void InitPlayerUI_Tex();

    /** \brief 인벤토리 무기 UI 초기화 */
    void InitInventoryWeaponTex();

    /** \brief 플레이어 무기 선택 UI 초기화 */
    void InitMainWeaponMat();

// 시작 후 초기화
public:
    /** \brief 플레이어 인벤토리 설정 */
    void InitPlayerInventory();

    /** \brief 플레이어 UI 띄움 */
    void SetPlayerUI();

    /** \brief 미구현 상태 */
    void Set_weapon_UI();

public:
    /** \brief 상호작용 UI 업데이트 (델리게이트) */
    UFUNCTION() void UpdateInteractionUI(class UWidgetComponent* WidgetComp, FString Title);

    UFUNCTION() void RunEffectAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type);

    static UTexture2D* GetTexture2D(FsSlotItemData ItemData);

    static UTexture2D* GetTexture2D(int Index, FString Type = "");

    static UMaterial* GetMaterial(int Index);
};