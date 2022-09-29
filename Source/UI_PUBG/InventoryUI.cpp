#include "InventoryUI.h"
#include "CharacterSlotUI.h"
#include "CustomDragDropOperation.h"
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
#include <Components/CanvasPanelSlot.h>

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();
    CurrentItemSlot->SetVisibility(ESlateVisibility::Hidden);
    
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
    {
        subGameInst->DeleSetTooltipVisibility.BindUFunction(this, "SetTooltipVisibility");
        subGameInst->DeleMoveSlotCursor.BindUFunction(this,"MoveSlotCursor");

    }
}

void UInventoryUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    CheckTooltipMouseDistance();
}

bool UInventoryUI::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

    if (auto p_customDragOp = Cast<UCustomDragDropOperation>(InOperation))
    {
        if (auto p_slot = p_customDragOp->GetSlot())
        {
            if (auto pGameInstanceSubsystemUI = UGameInstance::GetSubsystem<UGameInstanceSubsystemUI>(GetWorld()->GetGameInstance()))
                pGameInstanceSubsystemUI->DeleVerifyAttachmentSlot.ExecuteIfBound(nullptr);
        }
    }
    GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, "Dragging Inventory UI");
    return true;
}

FReply UInventoryUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    return FReply::Handled();
}

void UInventoryUI::SetTooltipVisibility(UItemSlotUI* pItemSlotUI, ESlateVisibility TooltipVisibility)
{
    if (pItemSlotUI)
    {
        TooltipUI->SetVisibility(TooltipVisibility);
        TooltipUI->SetData(pItemSlotUI->ItemData);
    }
}

UFUNCTION() void UInventoryUI::MoveSlotCursor(FVector2D NewPos)
{
    GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Yellow, "Draggin slot" + NewPos.ToString());
    // CurrentItemSlot->SetPositionInViewport(NewPos); 
    
    if (auto canvasSlot = Cast<UCanvasPanelSlot>(CurrentItemSlot->Slot))
        canvasSlot->SetPosition(NewPos);

    auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseCursorWidget(EMouseCursor::Default, CurrentItemSlot);
}

void UInventoryUI::CheckTooltipMouseDistance()
{
    auto p_subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (!p_subGameInst)
        return;

    bool bFirstSlot = false;
    auto distance   = p_subGameInst->GetDistanceBetweenSlotCursor(CurrentItemSlot, bFirstSlot);

    if (p_subGameInst->IsMouseLeftFromUI(distance, bFirstSlot))
        TooltipUI->SetVisibility(ESlateVisibility::Hidden);
}