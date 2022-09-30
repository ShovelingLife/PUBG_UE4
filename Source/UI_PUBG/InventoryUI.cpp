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
#include <Blueprint/WidgetLayoutLibrary.h>

UInventoryUI::UInventoryUI(const FObjectInitializer& Initializer) : Super(Initializer)
{
    /*auto slotUI = ConstructorHelpers::FClassFinder<UItemSlotUI>(TEXT("WidgetBlueprint'/Game/1_Blueprints/UI/BP_ItemSlotUI.BP_ItemSlotUI_C'"));

    if (slotUI.Succeeded())
        mSlotUIClass = slotUI.Class;*/
}

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
    {
        subGameInst->DeleSetTooltipVisibility.BindUFunction(this, "SetTooltipVisibility");
        subGameInst->DeleActivateCursorSlot.BindUFunction(this,"ActivateCursorSlot");
    }
    if (CurrentItemSlot)
        CurrentItemSlot->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    CheckTooltipMouseDistance();
    MoveSlotCursor();
}

FReply UInventoryUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    return FReply::Handled();
}

FReply UInventoryUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    /*if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
        mbClicked = true;*/

    return FReply::Handled();
}

FReply UInventoryUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    CurrentItemSlot->SetVisibility(ESlateVisibility::Hidden);
    return FReply::Handled();
}

bool UInventoryUI::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
    //auto p_customDragOp = Cast<UCustomDragDropOperation>(InOperation);

    //if (p_customDragOp)
    //{
    //    p_customDragOp->Init(CurrentItemSlot);

    //    if (auto pGameInstanceSubsystemUI = UGameInstanceSubsystemUI::GetInst())
    //        pGameInstanceSubsystemUI->DeleVerifyAttachmentSlot.ExecuteIfBound(nullptr);
    //}
    ////GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, "Dragging Inventory UI");
    //InOperation = p_customDragOp;
    GEngine->AddOnScreenDebugMessage(6, 1.f, FColor::Red, "Dragging Inventory UI");
    
    return true;
}

bool UInventoryUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    CurrentItemSlot->SetVisibility(ESlateVisibility::Hidden);
    return true;
}

void UInventoryUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    CurrentItemSlot->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryUI::SetTooltipVisibility(UItemSlotUI* pItemSlotUI, ESlateVisibility TooltipVisibility)
{
    /*if (pItemSlotUI)
    {
        TooltipUI->SetVisibility(TooltipVisibility);
        TooltipUI->SetData(pItemSlotUI->ItemData);
    }*/
}

void UInventoryUI::MoveSlotCursor()
{
    if (CurrentItemSlot->GetVisibility() == ESlateVisibility::Hidden)
        return;

    FVector2D mousePos     = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());    
    FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
    FVector2D finalPos{ mousePos.X + 38.5f,mousePos.Y + 0.5f };
    
    if (auto canvasSlot = Cast<UCanvasPanelSlot>(CurrentItemSlot->Slot))
        canvasSlot->SetPosition(finalPos);
}

void UInventoryUI::CheckTooltipMouseDistance()
{
    /*auto p_subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (!p_subGameInst)
        return;

    bool bFirstSlot = false;
    auto distance   = p_subGameInst->GetDistanceBetweenSlotCursor(CurrentItemSlot, bFirstSlot);

    if (p_subGameInst->IsMouseLeftFromUI(distance, bFirstSlot))
        TooltipUI->SetVisibility(ESlateVisibility::Hidden);*/
}

UItemSlotUI* UInventoryUI::ActivateCursorSlot(bool bActivated /* = false */) { CurrentItemSlot->SetVisibility(bActivated ? ESlateVisibility::Visible : ESlateVisibility::Hidden); return CurrentItemSlot; }