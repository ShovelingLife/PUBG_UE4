#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "Inventory_UI.generated.h"

class UInventory_list_UI;
class UCharacter_Slot_UI;
class UInventory_Weapon_Slot_UI;

UCLASS()
class UI_PUBG_API UInventory_UI : public UUserWidget
{
	GENERATED_BODY()

private:
    class AUI_manager* mp_UI_manager;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCharacter_Slot_UI*        Character_Slot_UI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventory_list_UI*        Inventory_list_UI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventory_Weapon_Slot_UI* Inventory_Weapon_Slot_UI;

    class UCursor_UI* p_icon_UI = nullptr;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;
};