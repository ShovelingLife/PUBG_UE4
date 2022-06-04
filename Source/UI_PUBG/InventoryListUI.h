/**
 * \file Inventory_list_UI.h
 *
 * \brief 인벤토리 리스트 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "SlotItemData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryListUI.generated.h"

class UImage;
class UListView;
class USizeBox;
class UTextBlock;

class ABaseInteraction;
class ACoreThrowableWeapon;
class UItemSlotUI;
class UGameInstanceSubsystemUI;

UCLASS()
class UI_PUBG_API UInventoryListUI : public UUserWidget
{
	GENERATED_BODY()

private:
    UPROPERTY() UItemSlotUI*    mpSlotObj = nullptr;

    float mWorldSizeBoxWidth     = 0.f;
    float mInventorySizeBoxWidth = 0.f;

public:
    /** \brief 월드 리스트 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* WorldTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*   WorldListSizeBox;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView*  WorldListView;

    /** \brief 인벤토리 리스트 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* InventoryTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*   InventoryListSizeBox;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView*  InventoryListView;

    /** \brief 기타 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* HighlightImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* SeparatorImg;

    UPROPERTY(EditDefaultsOnly, Category = ItemSlot) TSubclassOf<UItemSlotUI> BP_ItemSlotUI;
    UPROPERTY() class UGameInstanceSubsystemUI* pGameInstanceSubsystemUI;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    /**
      * \brief 리스트에 마우스 클릭 시 슬롯 정보 가져옴
      * \param InGeometry UI 정보 
      * \param InMouseEvent 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /**
      * \brief 마우스 움직일 시 선택 이미지 값에 따라 숨김
      * \param InGeometry UI 정보
      * \param InMouseEvent 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    /**
      * \brief 마우스가 UI에서 벗어남 따라서 선택 이미지 숨김
      * \param InMouseEvent 마우스 이벤트
     */
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    /**
      * \brief 드래그 중일 시 슬롯 커서 생성
      * \param InGeometry UI 정보
      * \param InMouseEvent 마우스 이벤트
      * \param OutOperation 드래그 드롭 객체
     */
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, class UDragDropOperation*& OutOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    /**
      * \brief 인벤토리 무기 UI로부터 드래그 후 드롭할 시 위치에 따라 월드-인벤토리 리스트에 넣음
      * \param InGeometry UI 정보
      * \param InMouseEvent 마우스 이벤트
      * \param Operation 드래그 드롭 객체
      * \return boolean 드롭 성공 여부
     */
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InMouseEvent, class UDragDropOperation* Operation) override;

private:
    /** * \brief 월드 및 인벤토리 사이즈 박스 넓이 구함 */
    void GetItemListWidth();

    UItemSlotUI* GetInitializedSlotUI(ABaseInteraction* pObj, FsSlotItemData ItemData);

    UItemSlotUI* GetMatchingItemFromList(FString ItemName) const;

public:
    UFUNCTION() void DeleteFromList();

    /**
      * \brief 리스트 내 아이템 슬롯에서 선택할 시 호출함 (델리게이트)
      * \param pSlotObj 아이템 슬롯
     */
    UFUNCTION() void CheckForHoveredItem(UItemSlotUI* pSlotObj);

    UFUNCTION() void SwapWeaponSlot(UItemSlotUI* pWeaponSlot);

    UFUNCTION() void ChangeItemCount(ABaseInteraction* pObj, bool bAdd = true);

    UFUNCTION() void SetItemOntoInventory(ABaseInteraction* pObj, bool bDeleteFromList = false);
    
    UFUNCTION() void SwapInventoryExplosive(ACoreThrowableWeapon* NewExplosive, ACoreThrowableWeapon* OldExplosive);
};