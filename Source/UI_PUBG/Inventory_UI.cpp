
#include "Inventory_UI.h"
#include "Item_Slot_UI.h"
#include "UI_manager.h"
#include "Kismet/GameplayStatics.h"

void UInventory_UI::NativeConstruct()
{
    Super::NativeConstruct();

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));
}

void UInventory_UI::NativeTick(const FGeometry& _geometry, float _delta_time)
{
    Super::NativeTick(_geometry, _delta_time);

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

}