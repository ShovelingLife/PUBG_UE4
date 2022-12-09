#pragma once

#include "CoreMinimal.h"
#include "SlotItemData.h"
#include "GameFramework/Actor.h"
#include "InventoryManager.generated.h"

class USceneComponent;
class UInventoryUI;
class UInventoryListUI;
class UItemSlotUI;

UCLASS()
class UI_PUBG_API AInventoryManager : public AActor
{
    GENERATED_BODY()

private:
    // 기본 씬 컴포넌트
    UPROPERTY(EditAnywhere, Category = SceneComp) 
    USceneComponent* SceneComp = nullptr;

    // 인벤토리 매니저 블프
    TSubclassOf<UUserWidget> BP_InventoryUI;

    bool mbUpdatingList = false;

public:
    // 현재 인벤토리
    TMap<FString, UItemSlotUI*> MapCurrentItems;

    // 인벤토리 UI
    UInventoryUI* pInventoryUI;

public:
    AInventoryManager();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

// 초기화 함수 모음
private:
    // 인벤토리 UI 초기화
    void InitInventoryUI();

    // 인벤토리 위젯 초기화
    void InitInventoryWidget();

    void CheckInventoryCapacity();

public:
    // 플레이어가 인벤토리를 열음 (델리게이트)
    UFUNCTION() 
    void OpenInventory();

    // 플레이어가 인벤토리를 열음 (델리게이트)
    UFUNCTION() 
    void CloseInventory();

    // 인벤토리로부터 총알 개수를 확인하는 함수
    UFUNCTION() 
    int GetBulletCount(FString BulletType);

    // 인벤토리로에서 1개만큼 아이템 차감
    UFUNCTION() 
    void DeleteInventoryItem(FString ItemType);

    UFUNCTION() 
    void SetInventoryCapacity(int Capacity);

public:
    UInventoryListUI* GetInventoryListUI() const;
};