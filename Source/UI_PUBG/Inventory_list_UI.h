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
#include "Blueprint/UserWidget.h"
#include "Inventory_list_UI.generated.h"

class AWeapon_manager;
class UItem_Slot_UI;

class UImage;
class UListView;
class USizeBox;
class UTextBlock;

UCLASS()
class UI_PUBG_API UInventory_list_UI : public UUserWidget
{
	GENERATED_BODY()

private:
    /** \brief 무기 매니저 */
    UPROPERTY() AWeapon_manager* mp_weapon_manager = nullptr;

    UPROPERTY() UItem_Slot_UI* p_slot_obj;
    float m_world_size_box_width     = 0.f;
    float m_inventory_size_box_width = 0.f;

public:
    /** \brief 월드 리스트 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* World_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*   World_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView*  World_list_view;

    /** \brief 인벤토리 리스트 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Inventory_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*   Inventory_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView*  Inventory_list_view;

    /** \brief 기타 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*    Highlight_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*    Separator_img;

    UPROPERTY(EditDefaultsOnly, Category = Item_slot) TSubclassOf<UItem_Slot_UI> p_item_slot_UI_class;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    /**
      * \brief 리스트에 마우스 클릭 시 슬롯 정보 가져옴
      * \param _geometry UI 정보 
      * \param _in_mouse_event 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonDown(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event) override;

    /**
      * \brief 마우스 움직일 시 선택 이미지 값에 따라 숨김
      * \param _geometry UI 정보
      * \param _in_mouse_event 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseMove(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event) override;

    /**
      * \brief 마우스가 UI에서 벗어남 따라서 선택 이미지 숨김
      * \param _in_mouse_event 마우스 이벤트
     */
    virtual void NativeOnMouseLeave(const FPointerEvent& _in_mouse_event) override;

    /**
      * \brief 드래그 중일 시 슬롯 커서 생성
      * \param _geometry UI 정보
      * \param _in_mouse_event 마우스 이벤트
      * \param _operation 드래그 드롭 객체
     */
    virtual void NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event, class UDragDropOperation*& _operation) override;

    /**
      * \brief 인벤토리 무기 UI로부터 드래그 후 드롭할 시 위치에 따라 월드-인벤토리 리스트에 넣음
      * \param _geometry UI 정보
      * \param _in_mouse_event 마우스 이벤트
      * \param _operation 드래그 드롭 객체
      * \return boolean 드롭 성공 여부
     */
    virtual bool NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _in_mouse_event, class UDragDropOperation* _operation) override;

private:
    /**
      * \brief 월드 및 인벤토리 사이즈 박스 넓이 구함 
     */
    void Get_item_list_width();

    /**
      * \brief 슬롯과 마우스간 뷰포트 기준 거리값을 구함
      * \return FVector2D 슬롯-마우스간 거리 값
     */
    FVector2D Get_distance_between_slot_cursor();

public:
    /**
      * \brief 리스트 내 아이템 슬롯에서 선택할 시 호출함 (델리게이트)
      * \param _p_slot_obj 아이템 슬롯
     */
    UFUNCTION()
    void Check_for_hovered_item(UItem_Slot_UI* _p_slot_obj);

    UFUNCTION()
    void Delete_from_list();

    UFUNCTION()
    void Swap_weapon_slot(UItem_Slot_UI* _p_weapon_slot);
};