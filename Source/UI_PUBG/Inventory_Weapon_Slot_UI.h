#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Input/Events.h"
#include "PUBG_UE4/Global.h"
#include "Inventory_Weapon_Slot_UI.generated.h"

UCLASS()
class UI_PUBG_API UInventory_Weapon_Slot_UI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    class AUI_manager*     mp_UI_manager           = nullptr;
    class AWeapon_manager* mp_weapon_manager       = nullptr;
    Fs_slot_item_data      m_item_data;
          int              m_selected_weapon_index = 0;
          bool             m_is_clicked            = false;
          bool             m_is_initialized        = false;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Highlight_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Main_canvas_panel;
            
    //1¹ø ½½·Ô    
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* First_gun_canvas_panel;       
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       First_gun_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_bullet_type_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   First_gun_number_txt;

    //2¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Second_gun_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Second_gun_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_bullet_type_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Second_gun_number_txt;

    //3¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Pistol_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Pistol_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_bullet_type_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_magazine_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Pistol_number_txt;

    //4¹ø ½½·Ô (±ÙÁ¢¹«±â)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Melee_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Melee_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Melee_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Melee_number_txt;

    //5¹ø ½½·Ô (ÅõÃ´¹«±â)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* Grenade_canvas_panel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Grenade_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Grenade_name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Grenade_number_txt;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

    virtual void NativeOnMouseEnter(const FGeometry&, const FPointerEvent&) override;

    virtual void NativeOnMouseLeave(const FPointerEvent&) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;

    virtual FReply NativeOnMouseButtonUp(const FGeometry&, const FPointerEvent&) override;

    virtual void NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _pointer_event, class UDragDropOperation*& _operation) override;

    virtual bool NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, class UDragDropOperation* _operation) override;

private:
    void Update_UI_visibility();

    void Update_inventory_weapon_UI();

    void Update_weapon_UI_highlight_img();

    void Reset_highlight_img();

    class ABase_interaction* Get_weapon();

public:
    // ½½·Ô ÃÊ±âÈ­
    UFUNCTION()
    void Set_slot_null();
};