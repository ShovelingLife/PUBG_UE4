#include "CustomDragDropOperation.h"
#include "ItemSlotUI.h"

void UCustomDragDropOperation::Init(UItemSlotUI* pSlot, FString Type /* = "" */)
{
    pSlotUI = pSlot;
    DefaultDragVisual = pSlot;
    Pivot = EDragPivot::MouseDown;
    bWeaponSlot = (Type == "Weapon");
    bInventoryList = (Type == "Inventory");
}