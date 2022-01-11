#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory_manager.generated.h"

UCLASS()
class UI_PUBG_API AInventory_manager : public AActor
{
    GENERATED_BODY()

private:
    // UI ����
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

    // �ʱ�ȭ �Լ� ����
private:

    // �κ��丮 ���� �ʱ�ȭ
    void Init_inventory_widget();

    // ��Ÿ �Լ� ����
private:
    // �κ��丮 ���¸� ������Ʈ
    void Check_inventory_state();
};