#include "Item_Slot_UI.h"
#include "UI_manager.h"
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
}

void UItem_Slot_UI::NativeOnListItemObjectSet(UObject* _p_obj)
{
    auto p_tmp_slot   = Cast<UItem_Slot_UI>(_p_obj);

    if (p_tmp_slot)
    {
        Item_img->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::map_inventory_weapon_ui_tex[p_tmp_slot->item_data.image_index]));
        Name_txt->SetText(FText::FromString(p_tmp_slot->item_data.name));

        if (item_data.count > 0)
            Count_txt->SetText(FText::FromString(FString::FromInt(p_tmp_slot->item_data.count)));
    }
}

void UItem_Slot_UI::NativeOnItemSelectionChanged(bool _is_selected)
{
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