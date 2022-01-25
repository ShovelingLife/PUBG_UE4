#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Inventory_list_UI.generated.h"

UCLASS()
class UI_PUBG_API UInventory_list_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 드랍 아이템 리스트
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UListView* World_list_view;

    // 현재 인벤토리 아이템 리스트
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UListView* Inventory_list_view;
};
