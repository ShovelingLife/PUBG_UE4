/**
 * \file Inventory_manager.h
 *
 * \brief 인벤토리 매니저
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "SlotItemData.h"
#include "GameFramework/Actor.h"
#include "InventoryManager.generated.h"

class USceneComponent;
class UInventoryUI;
class UInventoryListUI;

UCLASS()
class UI_PUBG_API AInventoryManager : public AActor
{
    GENERATED_BODY()

private:
    /** \brief 기본 씬 컴포넌트 */
    UPROPERTY(EditAnywhere, Category = SceneComp) USceneComponent* SceneComp = nullptr;

    /** \brief 인벤토리 매니저 블프 */
    TSubclassOf<UUserWidget> BP_InventoryUI;

public:
    /** \brief 인벤토리 UI */
    class UInventoryUI* pInventoryUI;

public:
    AInventoryManager();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

// 초기화 함수 모음
private:
    /** \brief 인벤토리 UI 초기화 */
    void InitInventoryUI();

    /** \brief 인벤토리 위젯 초기화 */
    void InitInventoryWidget();

public:
    /** \brief 플레이어가 인벤토리를 열음 (델리게이트) */
    UFUNCTION() void OpenInventory();

    /** \brief 플레이어가 인벤토리를 열음 (델리게이트) */
    UFUNCTION() void CloseInventory();

public:
    UInventoryListUI* GetInventoryListUI();
};