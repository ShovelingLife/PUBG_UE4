#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Character_Slot_UI.h"
#include "Inventory_Weapon_Slot_UI.h"
#include "Inventory_UI.generated.h"

UCLASS()
class UI_PUBG_API UInventory_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    // 드랍 아이템 리스트
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	    UListView* World_list_view;

    // 현재 인벤토리 아이템 리스트
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UListView* Inventory_list_view;

    // 캐릭터 장비창 UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UCharacter_Slot_UI* BP_UI_Character_Slot;

    // 캐릭터 무기 창
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
       UInventory_Weapon_Slot_UI* BP_UI_Inventory_Weapon_Slot;
};