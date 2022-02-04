#include "Inventory_UI.h"
#include "Character_Slot_UI.h"
#include "Inventory_list_UI.h"
#include "Item_Slot_UI.h"
#include "Inventory_Weapon_Slot_UI.h"
#include "Inventory_manager.h"
#include "UI_manager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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

FReply UInventory_UI::NativeOnMouseButtonDown(const FGeometry& _geometry, const FPointerEvent& _mouse_event)
{
    Super::NativeOnMouseButtonDown(_geometry, _mouse_event);
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(_mouse_event, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UInventory_UI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    return FReply::Handled();
}