#include "InventoryUI.h"
#include "CharacterSlotUI.h"
#include "InventorylistUI.h"
#include "ItemSlotUI.h"
#include "InventoryWeaponSlotUI.h"
#include "Inventorymanager.h"
#include "UI_manager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (!mpUI_manager)
        mpUI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));
}

void UInventoryUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);
}