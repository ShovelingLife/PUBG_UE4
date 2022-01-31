#include "Item_Slot_UI.h"
#include "UI_manager.h"
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
    auto p_tmp_item   = Cast<UItem_Slot_UI>(_p_obj);

    if (p_tmp_item)
    {
        Item_img->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::map_inventory_weapon_ui_tex[p_tmp_item->item_data.image_index]));
        Name_txt->SetText(FText::FromString(p_tmp_item->item_data.name));
        Count_txt->SetText(FText::FromString(FString::FromInt(p_tmp_item->item_data.count)));
        item_data = Fs_slot_item_data();
    }
}

void UItem_Slot_UI::NativeOnItemSelectionChanged(bool _is_selected)
{
}

void UItem_Slot_UI::Init()
{
    Item_img  = NewObject<UImage>();
    Name_txt  = NewObject<UTextBlock>();
    Count_txt = NewObject<UTextBlock>();
}