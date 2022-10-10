#include "CustomDragDropOperation.h"
#include "ItemSlotUI.h"
#include "GameInstanceSubsystemUI.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "Blueprint/UserWidget.h"
#include <Kismet/KismetMathLibrary.h>

void UCustomDragDropOperation::Init(UItemSlotUI* pSlot, FVector2D Pos)
{
    if (!pSlot)
        return;
    
    Payload = pSlot;
    Offset = FVector2D::ZeroVector;
    Pivot = EDragPivot::MouseDown;
}

void UCustomDragDropOperation::Classify(FString Type)
{
    bWeaponSlot    = (Type == "Weapon");
    bInventoryList = (Type == "Inventory");
}

UItemSlotUI* UCustomDragDropOperation::GetSlot() const { return Cast<UItemSlotUI>(Payload); }