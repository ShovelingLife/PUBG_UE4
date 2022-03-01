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
#include "UI_manager.generated.h"

class UPlayerUI;
class UItemSlotUI;
class AInventoryManager;
class UTooltipUI;

UCLASS()
class UI_PUBG_API AUI_manager : public AActor
{
	GENERATED_BODY()

private:
    // ------- 현재 쓰이고있는 변수들 -------

    /** * \brief 플레이어 UI BP */
    TSubclassOf<UPlayerUI> mPlayerUI_BP;

    /** * \brief 플레이어 UI */
    UPROPERTY() UPlayerUI* mpPlayer_UI;

    /** * \brief 상호작용 UI 관련 */
    UPROPERTY(VisibleAnywhere, Category = WidgetBP) TSubclassOf<UUserWidget> mInteractionWidgetBP;

    /** * \brief 메인 플레이어 UI 리소스 경로명 */
    const TArray<FString> mkArrPlayerUI_TexPath 
    {
        "SingleShot",
        "BurstShot",
        "ConsecutiveShot",
    };

    const FString mkDefaultPlayerUI_path = "/Game/UI/Player/State/";

public:
    static TMap<int, UMaterial*> MapMainWeaponMat;      // 메인 무기 UI    
    static TMap<int, UTexture*>  MapInventoryWeaponTex; // 인벤토리 무기 UI    
    static TMap<int, UTexture*>  MapPlayerTex;          // 플레이어 UI

    UPROPERTY() AInventoryManager* pInventoryManager;

public:
    AUI_manager();

protected:
    virtual void BeginPlay() override;

// 생성자 내 초기화 함수
private:
    /**
     * \brief 최상위 플레이어 UI 초기화
     */
    void InitPlayerUI();

    /**
     * \brief 상호작용 UI 초기화
     */
    void InitInteractionUI();

    /**
     * \brief 플레이어 UI 초기화
     */
    void InitPlayerUI_Tex();

    /**
     * \brief 인벤토리 무기 UI 초기화
     */
    void InitInventoryWeaponTex();

    /**
     * \brief 플레이어 무기 선택 UI 초기화
     */
    void InitMainWeaponMat();

// 시작 후 초기화
public:
    /**
     * \brief 플레이어 인벤토리 설정
     */
    void InitPlayerInventory();

    /**
     * \brief 플레이어 UI 띄움
     */
    void SetPlayerUI();

    /**
     * \brief 미구현 상태
     */
    void Set_weapon_UI();

public:
    /**
     * \brief 상호작용 UI 업데이트 (델리게이트)
     */
    UFUNCTION()
    void UpdateInteractionUI(class UWidgetComponent* _widget_comp, FString _title);
};