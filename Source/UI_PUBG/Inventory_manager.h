#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/Global.h"
#include "Inventory_manager.generated.h"

UCLASS()
class UI_PUBG_API AInventory_manager : public AActor
{
    GENERATED_BODY()

private:
    class AUI_manager* mp_UI_manager;

    UPROPERTY(EditAnywhere, Category = Scene_comp)
        class USceneComponent* mp_scene_comp = nullptr;

    // UI 관련
    TSubclassOf<UUserWidget> m_inventory_actor;

public:
    class UInventory_UI* p_inventory_UI;
    bool is_opened = false;

public:
    AInventory_manager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    // 초기화 함수 모음
private:
    void Init_inventory_UI();

    // 인벤토리 위젯 초기화
    void Init_inventory_widget();

public:
    // 인벤토리를 열음
    UFUNCTION()
    void Open_inventory();

    UFUNCTION()
    void Close_inventory();

    UFUNCTION()
    void Update_item_slot_UI(Fs_slot_item_data _item_data);
};