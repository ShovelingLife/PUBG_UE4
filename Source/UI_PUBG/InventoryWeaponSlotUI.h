/**
 * \file Inventory_Weapon_Slot_UI.h
 *
 * \brief 인벤토리 무기 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Events.h"
#include "ItemSlotUI.h"
#include "Player_weapons/WeaponEnum.h"
#include "InventoryWeaponSlotUI.generated.h"

class AUI_manager;
class AWeaponManager;
class UItemSlotUI;

class UBorder;
class UCanvasPanel;
class UImage;
class UTextBlock;

UCLASS()
class UI_PUBG_API UInventoryWeaponSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /** \brief UI 매니저 */
    UPROPERTY() AUI_manager*    mpUI_manager    = nullptr;

    /** \brief 무기 매니저 */
    UPROPERTY() AWeaponManager* mpWeaponManager = nullptr;

    /**
      * \brief 현재 쓰여지는 변수
     */
    Fs_SlotItemData  mItemData;
    ECurrentWeaponType mSelectedWeaponIndex;
    ECurrentWeaponType mDraggedWeaponIndex;
    bool               mbClicked            = false;

public:
    /**
      * \brief 선택 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       HighlightImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* MainCanvasPanel;
             
    /**
      * \brief 1번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* FirstGunCanvasPanel;       
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      FirstGunNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunBulletTypeTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunCurrentMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunMaxMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       FirstGunSlotImg;

    /**
      * \brief 2번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* SecondGunCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      SecondGunNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunBulletTypeTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunCurrentMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunMaxMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       SecondGunSlotImg;

    /**
      * \brief 3번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* PistolCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      PistolNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolBulletTypeTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolCurrentMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolMaxMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       PistolSlotImg;

    /**
      * \brief 4번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* MeleeCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      MeleeNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   MeleeNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       MeleeSlotImg;

    /**
      * \brief 5번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* GrenadeCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      GrenadeNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   GrenadeNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       GrenadeSlotImg;

    /** \brief 아이템 슬롯 BP */
    UPROPERTY(EditDefaultsOnly, Category = Item_slot) TSubclassOf<UItemSlotUI> BP_itemSlotUI;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

    /**
      * \brief 마우스가 UI를 벗어날 시 선택 이미지 초기화
      * \param _in_mouse_event 마우스 이벤트
     */
    virtual void NativeOnMouseLeave(const FPointerEvent& _InMouseEvent) override;

    /**
      * \brief 좌클릭 시 (선택된 UI에 맞게끔 슬롯 변환 > 드래그) / 우클릭 시 맵에 드롭
      * 선택 이미지 초기화
      * \param _InGeometry UI 정보 
      * \param _InMouseEvent 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;
    
    /**
      * \brief 마우스 버튼 땠을 시 선택 이미지 및 클릭된 UI 정보 삭제
      * \param _in_geometry UI 정보 \param _in_mouse_event 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonUp(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent, class UDragDropOperation*& _InOperation) override;

    virtual bool NativeOnDrop(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, class UDragDropOperation* _InOperation) override;

    virtual void NativeOnDragEnter(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

    virtual void NativeOnDragLeave(const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

    virtual bool NativeOnDragOver(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

private:
    /**
      * \brief UI 상태 업데이트
     */
    void UpdateVisibility();

    void UpdateInventoryWeaponUI();

    void CheckForHoveredWeaponSlot();

    void UpdateHighlightImgPos();

    void ResetHighlightImg();

    void GetIemData(ABaseInteraction* _pWeapon);

public:
    // 슬롯 초기화
    UFUNCTION()
    void SetSlotNull();
};