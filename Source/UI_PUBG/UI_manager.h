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
    static TMap<int, UMaterial*> map_main_weapon_ui_mat; // 메인 무기 UI    
    static TMap<int, UTexture*> map_inventory_weapon_ui_tex; // 인벤토리 무기 UI    
    static TMap<int, UTexture*> map_player_ui_tex; // 플레이어 UI

private:
    // ------- 현재 쓰이고있는 변수들 -------

    class AGlobal*      mp_global = nullptr;

    // 최상위 UI 모든걸 포함
    UPlayer_UI*             mp_player_UI;
    TSubclassOf<UPlayer_UI> m_bp_player_UI;

    // 인벤토리
    TSubclassOf<AActor> m_inventory_manager_actor;

    // 상호작용 UI 관련
    UPROPERTY(VisibleAnywhere, Category = Widget_bp)
        TSubclassOf<UUserWidget> m_interaction_widget_bp;

    // 메인 플레이어 UI 리소스 경로명
    const TArray<FString> mk_arr_player_UI_tex_path 
    {
        "Single_shot",
        "Burst_shot",
        "Consecutive_shot",
    };

    const FString mk_default_player_UI_path = "/Game/UI/Player/State/";

public:
    // 아이템 슬롯 UI
    UItem_Slot_UI*             p_item_slot_UI;
    TSubclassOf<UItem_Slot_UI> bp_item_slot_UI;

    class AInventory_manager* p_inventory_manager;

public:
    // Sets default values for this actor's properties
    AUI_manager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 생성자 내 초기화 함수
private:

    // 최상위 플레이어 UI 초기화
    void Init_player_UI();

    // 상호작용 UI 초기화
    void Init_interaction_UI();

    void Init_slot_UI();

    // 플레이어 UI를 띄움
    void Set_player_UI();

    // 플레이어 인벤토리를 설정
    void Init_player_inventory();

    void Init_inventory_weapon_UI_tex();

    void Init_main_weapon_UI_mat();

    // 플레이어 UI 초기화
    void Init_player_UI_tex();

    void Set_weapon_UI();

    void Set_inventory_slot_UI();

public:
    UFUNCTION()
    void Update_interaction_UI(class UWidgetComponent* _widget_comp, FString _title);
};