#include "InventoryUI.h"
#include "CharacterSlotUI.h"
#include "GameInstanceSubsystemUI.h"
#include "InventorylistUI.h"
#include "Inventorymanager.h"
#include "InventoryWeaponSlotUI.h"
#include "ItemSlotUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
        subGameInst->DeleSetTooltipVisibility.BindUFunction(this, "SetTooltipVisibility");
}

void UInventoryUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    CheckTooltipMouseDistance();
}

void UInventoryUI::SetTooltipVisibility(UItemSlotUI* pItemSlotUI, ESlateVisibility TooltipVisibility)
{
    if (pItemSlotUI)
    {
        TooltipUI->SetVisibility(TooltipVisibility);
        TooltipUI->SetData(pItemSlotUI->ItemData);
        mpCurrentItemSlot = pItemSlotUI;
    }
}

void UInventoryUI::CheckTooltipMouseDistance()
{
    auto p_subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (!p_subGameInst)
        return;

    bool bFirstSlot = false;
    auto distance   = p_subGameInst->GetDistanceBetweenSlotCursor(mpCurrentItemSlot, bFirstSlot);

    if (p_subGameInst->IsMouseLeftFromUI(distance, bFirstSlot))
        TooltipUI->SetVisibility(ESlateVisibility::Hidden);
}