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
    auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (subGameInst)
        subGameInst->DeleHideTooltip.BindUFunction(this, "SetTooltipVisibility");

    if (!mpUI_manager)
        mpUI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));
}

void UInventoryUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);
}

void UInventoryUI::SetTooltipVisibility(ESlateVisibility _Visibility)
{
    TooltipUI->SetVisibility(_Visibility);
}