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
#include "GameFramework/Actor.h"
#include "Inventory_manager.generated.h"

class USceneComponent;
class UInventory_list_UI;

UCLASS()
class UI_PUBG_API AInventory_manager : public AActor
{
    GENERATED_BODY()

private:
    /** \brief 기본 씬 컴포넌트 */
    UPROPERTY(EditAnywhere, Category = Scene_comp) USceneComponent* mp_scene_comp = nullptr;

    /** \brief 인벤토리 매니저 블프 */
    TSubclassOf<UUserWidget> m_inventory_actor;

public:
    /** \brief 인벤토리 UI */
    class UInventory_UI* p_inventory_UI;

public:
    AInventory_manager();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

// 초기화 함수 모음
private:
    /**
      * \brief 인벤토리 UI 초기화 
     */
    void Init_inventory_UI();

    /**
      * \brief 인벤토리 위젯 초기화 
     */
    void Init_inventory_widget();

public:
    /**
      * \brief 플레이어가 인벤토리를 열음 (델리게이트)
     */
    UFUNCTION()
    void Open_inventory();

    /**
      * \brief 플레이어가 인벤토리를 열음 (델리게이트)
     */
    UFUNCTION()
    void Close_inventory();
};