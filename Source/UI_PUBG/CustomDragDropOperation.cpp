#include "CustomDragDropOperation.h"
#include "ItemSlotUI.h"
#include "PUBG_UE4/BaseInteraction.h"

void UCustomDragDropOperation::Init(UItemSlotUI* pSlot, FString Type /* = "" */)
{
    mCurrentSlot = pSlot;
    Payload = pSlot;
    Pivot = EDragPivot::MouseDown;
    bWeaponSlot = (Type == "Weapon");
    bInventoryList = (Type == "Inventory");
}

UItemSlotUI* UCustomDragDropOperation::GetSlot() const
{
    return Cast<UItemSlotUI>(Payload);
}