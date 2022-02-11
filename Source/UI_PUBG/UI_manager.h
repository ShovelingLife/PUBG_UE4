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

class UPlayer_UI;
class UItem_Slot_UI;

UCLASS()
class UI_PUBG_API AUI_manager : public AActor
{
	GENERATED_BODY()
	
public:    
    static TMap<int, UMaterial*> map_main_weapon_ui_mat;      // 메인 무기 UI    
    static TMap<int, UTexture*>  map_inventory_weapon_ui_tex; // 인벤토리 무기 UI    
    static TMap<int, UTexture*>  map_player_ui_tex;           // 플레이어 UI

private:
    // ------- 현재 쓰이고있는 변수들 -------

    /** * \brief 플레이어 UI */
    UPlayer_UI*             mp_player_UI;

    /** * \brief 플레이어 UI BP */
    TSubclassOf<UPlayer_UI> m_bp_player_UI;

    /** * \brief 인벤토리 */
    TSubclassOf<AActor> m_inventory_manager_actor;

    /** * \brief 상호작용 UI 관련 */
    UPROPERTY(VisibleAnywhere, Category = Widget_bp) TSubclassOf<UUserWidget> m_interaction_widget_bp;

    /** * \brief 메인 플레이어 UI 리소스 경로명 */
    const TArray<FString> mk_arr_player_UI_tex_path 
    {
        "Single_shot",
        "Burst_shot",
        "Consecutive_shot",
    };

    const FString mk_default_player_UI_path = "/Game/UI/Player/State/";

public:
    class AInventory_manager* p_inventory_manager;

public:
    AUI_manager();

protected:
    virtual void BeginPlay() override;

// 생성자 내 초기화 함수
private:
    /**
     * \brief 최상위 플레이어 UI 초기화
     */
    void Init_player_UI();

    /**
     * \brief 상호작용 UI 초기화
     */
    void Init_interaction_UI();

    /**
     * \brief 플레이어 UI 띄움
     */
    void Set_player_UI();

    /**
     * \brief 플레이어 인벤토리 설정
     */
    void Init_player_inventory();

    /**
     * \brief 인벤토리 무기 UI 초기화
     */
    void Init_inventory_weapon_UI_tex();

    /**
     * \brief 플레이어 무기 선택 UI 초기화
     */
    void Init_main_weapon_UI_mat();

    /**
     * \brief 플레이어 UI 초기화
     */
    void Init_player_UI_tex();

    /**
     * \brief 미구현 상태
     */
    void Set_weapon_UI();

public:
    /**
     * \brief 상호작용 UI 업데이트 (델리게이트)
     */
    UFUNCTION()
    void Update_interaction_UI(class UWidgetComponent* _widget_comp, FString _title);
};