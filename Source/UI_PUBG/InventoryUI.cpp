#include "InventoryUI.h"
#include "CharacterSlotUI.h"
#include "GameInstanceSubsystemUI.h"
#include "InventorylistUI.h"
#include "Inventorymanager.h"
#include "InventoryWeaponSlotUI.h"
#include "ItemSlotUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
        subGameInst->DeleSetTooltipVisibility.BindUFunction(this, "SetTooltipVisibility");

    /*if (!mpUI_manager)
        mpUI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));*/
}

void UInventoryUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);
    CheckTooltipMouseDistance();
}

void UInventoryUI::SetTooltipVisibility(UItemSlotUI* _pItemSlotUI, ESlateVisibility _Visibility)
{
    TooltipUI->SetVisibility(_Visibility);
    TooltipUI->SetData(_pItemSlotUI);
    mpCurrentItemSlot = _pItemSlotUI;
}

void UInventoryUI::CheckTooltipMouseDistance()
{
    auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (!subGameInst)
        return;

    bool bFirst = false;
    auto distance = subGameInst->GetDistanceBetweenSlotCursor(mpCurrentItemSlot, bFirst);

    if (subGameInst->IsMouseLeftFromUI(distance, bFirst))
        TooltipUI->SetVisibility(ESlateVisibility::Hidden);
}