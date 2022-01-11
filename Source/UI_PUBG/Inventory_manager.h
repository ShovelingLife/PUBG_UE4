#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory_manager.generated.h"

UCLASS()
class UI_PUBG_API AInventory_manager : public AActor
{
    GENERATED_BODY()

private:
    // UI 관련
    TSubclassOf<UUserWidget> m_inventory_actor;
    class UInventory_UI* mp_inventory_ui;

    class ACustom_player* mp_custom_player;

public:
    bool is_opened = false;

public:
    AInventory_manager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 초기화 함수 모음
private:

    // 인벤토리 위젯 초기화
    void Init_inventory_widget();

    // 기타 함수 모음
private:
    // 인벤토리 상태를 업데이트
    void Check_inventory_state();
};