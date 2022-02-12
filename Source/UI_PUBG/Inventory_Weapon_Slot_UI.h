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
#include "Item_Slot_UI.h"
#include "Inventory_Weapon_Slot_UI.generated.h"

class AUI_manager;
class AWeapon_manager;
class UImage;
class UCanvasPanel;
class UTextBlock;
class UItem_Slot_UI;
class UBorder;

UCLASS()
class UI_PUBG_API UInventory_Weapon_Slot_UI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /** \brief UI 매니저 */
    UPROPERTY() AUI_manager*     mp_UI_manager     = nullptr;

    /** \brief 무기 매니저 */
    UPROPERTY() AWeapon_manager* mp_weapon_manager = nullptr;

    /**
      * \brief 현재 쓰여지는 변수
     */
    Fs_slot_item_data m_item_data;
    int               m_selected_weapon_index = 0;
    bool              m_is_clicked            = false;

public:
    /**
      * \brief 선택 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Highlight_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Main_canvas_panel;
             
    /**
      * \brief 1번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* First_gun_canvas_panel;       
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      First_gun_number_background;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_bullet_type_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_current_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_max_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       First_gun_slot_img;

    /**
      * \brief 2번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Second_gun_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      Second_gun_number_background;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_bullet_type_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_current_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_max_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Second_gun_slot_img;

    /**
      * \brief 3번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Pistol_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      Pistol_number_background;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_bullet_type_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_current_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_max_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Pistol_slot_img;

    /**
      * \brief 4번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Melee_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      Melee_number_background;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Melee_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Melee_slot_img;

    /**
      * \brief 5번 슬롯 UI 관련
     */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Grenade_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      Grenade_number_background;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Grenade_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Grenade_slot_img;

    /** \brief 아이템 슬롯 BP */
    UPROPERTY(EditDefaultsOnly, Category = Item_slot) TSubclassOf<UItem_Slot_UI> p_item_slot_UI_class;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

    /**
      * \brief 마우스가 UI를 벗어날 시 선택 이미지 초기화
      * \param _in_mouse_event 마우스 이벤트
     */
    virtual void NativeOnMouseLeave(const FPointerEvent&) override;

    /**
      * \brief 좌클릭 시 (선택된 UI에 맞게끔 슬롯 변환 > 드래그) / 우클릭 시 맵에 드롭
      * 선택 이미지 초기화
      * \param _geometry UI 정보 \param _in_mouse_event 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonDown(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event) override;
    
    /**
      * \brief 마우스 버튼 땠을 시 선택 이미지 및 클릭된 UI 정보 삭제
      * \param _geometry UI 정보 \param _in_mouse_event 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonUp(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event) override;

    virtual void NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _pointer_event, class UDragDropOperation*& _operation) override;

    virtual bool NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, class UDragDropOperation* _operation) override;

    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    /**
      * \brief UI 상태 업데이트
     */
    void Update_UI_visibility();

    void Update_inventory_weapon_UI();

    void Update_weapon_UI_highlight_img();

    void Reset_highlight_img();

public:
    // 슬롯 초기화
    UFUNCTION()
    void Set_slot_null();
};