#include "Inventory_UI.h"
#include "Item_Slot_UI.h"
#include "Inventory_manager.h"
#include "UI_manager.h"
#include "Kismet/GameplayStatics.h"

void UInventory_UI::NativeConstruct()
{
    Super::NativeConstruct();

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));
}

void UInventory_UI::NativeTick(const FGeometry& _geometry, float _delta_time)
{
    Super::NativeTick(_geometry, _delta_time);
}

FReply UInventory_UI::NativeOnKeyDown(const FGeometry& _geometry, const FKeyEvent& _key_event)
{
    Super::NativeOnKeyDown(_geometry, _key_event);

    if (!mp_UI_manager ||
        !mp_UI_manager->p_inventory_manager)
        return FReply::Unhandled();

    auto p_inventory_manager = mp_UI_manager->p_inventory_manager;
    FString str_key = _key_event.GetKey().GetFName().ToString();

    //// 인벤토리를 닫음
    //if      (str_key == "Tab")
    //         p_inventory_manager->Close_inventory();

    return FReply::Handled();
}