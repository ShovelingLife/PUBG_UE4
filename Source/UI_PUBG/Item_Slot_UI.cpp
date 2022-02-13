#include "Item_Slot_UI.h"
#include "Custom_drag_drop_operation.h"
#include "UI_manager.h"
#include "PUBG_UE4/Custom_game_instance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/PanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UItem_Slot_UI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UItem_Slot_UI::NativeTick(const FGeometry& _geometry, float _delta_time)
{
    Super::NativeTick(_geometry, _delta_time);

    if (this->IsHovered())
        dele_check_for_slot.ExecuteIfBound(this);
}

void UItem_Slot_UI::NativeOnListItemObjectSet(UObject* _p_obj)
{
    auto p_slot = Cast<UItem_Slot_UI>(_p_obj);

    if (p_slot)
    {
        Item_img->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::map_inventory_weapon_ui_tex[p_slot->item_data.image_index]));
        Name_txt->SetText(FText::FromString(p_slot->item_data.name));
        p_dragged_item            = p_slot->p_dragged_item;
        dele_check_for_slot       = p_slot->dele_check_for_slot;
        dele_swap_weapon_slot     = p_slot->dele_swap_weapon_slot;
        dele_set_weapon_slot_null = p_slot->dele_set_weapon_slot_null;
        item_data                 = p_slot->item_data;

        if (item_data.count > 1)
            Count_txt->SetText(FText::FromString(FString::FromInt(p_slot->item_data.count)));
    }
}

void UItem_Slot_UI::Set_as_cursor(FVector2D _pos)
{
    auto p_canvas_panel_slot = Cast<UCanvasPanelSlot>(Main_horizontal_box->Slot);
    
    if (p_canvas_panel_slot)
        p_canvas_panel_slot->SetPosition(_pos);

    this->SetVisibility(ESlateVisibility::Visible);
    Item_img->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::map_inventory_weapon_ui_tex[item_data.image_index]));
    Name_txt->SetVisibility(ESlateVisibility::Hidden);
    Count_txt->SetVisibility(ESlateVisibility::Hidden);
    Background_size_box->SetVisibility(ESlateVisibility::Hidden);
}