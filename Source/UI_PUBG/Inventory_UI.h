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
    // ��� ������ ����Ʈ
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	    UListView* World_list_view;

    // ���� �κ��丮 ������ ����Ʈ
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UListView* Inventory_list_view;

    // ĳ���� ���â UI
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UCharacter_Slot_UI* BP_UI_Character_Slot;

    // ĳ���� ���� â
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
       UInventory_Weapon_Slot_UI* BP_UI_Inventory_Weapon_Slot;
};