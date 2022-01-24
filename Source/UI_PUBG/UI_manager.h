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
	
private:
    // ------- ���� ���̰��ִ� ������ -------

    class AGlobal*      mp_global = nullptr;

    // �ֻ��� UI ���� ����
    UPlayer_UI*             mp_player_UI;
    TSubclassOf<UPlayer_UI> m_bp_player_UI;

    // �κ��丮
    TSubclassOf<AActor> m_inventory_manager_actor;

    // ��ȣ�ۿ� UI ����
    UPROPERTY(VisibleAnywhere, Category = Widget_bp)
        TSubclassOf<UUserWidget> m_interaction_widget_bp;

    // ���� �÷��̾� UI ���ҽ� ��θ�
    const TArray<FString> mk_arr_player_UI_tex_path 
    {
        "Single_shot",
        "Burst_shot",
        "Consecutive_shot",
    };

    const FString mk_default_player_UI_path = "/Game/UI/Player/State/";

public:
    // ������ ���� UI
    UItem_Slot_UI*             p_item_slot_UI;
    TSubclassOf<UItem_Slot_UI> bp_item_slot_UI;

    // ���� ���� UI
    UPROPERTY(VisibleAnywhere, Category = Weapon_UI)
        TMap<int, UMaterial*> map_main_weapon_ui_mat;

    // �κ��丮 ���� UI
    UPROPERTY(VisibleAnywhere, Category = Weapon_UI)
        TMap<int, UTexture*> map_inventory_weapon_ui_tex;

    UPROPERTY(VisibleAnywhere, Category = Weapon_UI)
        TMap<int, UTexture*> map_player_ui_tex;

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

    // ������ �� �ʱ�ȭ �Լ�
private:

    // �ֻ��� �÷��̾� UI �ʱ�ȭ
    void Init_player_UI();

    // ��ȣ�ۿ� UI �ʱ�ȭ
    void Init_interaction_UI();

    void Init_slot_UI();

    // �÷��̾� UI�� ���
    void Set_player_UI();

    // �÷��̾� �κ��丮�� ����
    void Init_player_inventory();

    void Init_inventory_weapon_UI_tex();

    void Init_main_weapon_UI_mat();

    // �÷��̾� UI �ʱ�ȭ
    void Init_player_UI_tex();

    void Set_weapon_UI();

    void Set_inventory_slot_UI();

public:
    UFUNCTION()
    void Update_interaction_UI(class UWidgetComponent* _widget_comp, FString _title);
};