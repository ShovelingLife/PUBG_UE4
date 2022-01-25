#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character_Slot_UI.h"
#include "Inventory_list_UI.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "Inventory_Weapon_Slot_UI.h"
#include "Inventory_UI.generated.h"

UCLASS()
class UI_PUBG_API UInventory_UI : public UUserWidget
{
	GENERATED_BODY()

private:
    class AUI_manager* mp_UI_manager;

public:
    // 리스트 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UInventory_list_UI* Inventory_list_UI;

    // 캐릭터 장비창 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UCharacter_Slot_UI* Character_Slot_UI;

    // 캐릭터 무기 창
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
       UInventory_Weapon_Slot_UI* Inventory_Weapon_Slot_UI;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    virtual FReply NativeOnKeyDown(const FGeometry&, const FKeyEvent&) override;
};