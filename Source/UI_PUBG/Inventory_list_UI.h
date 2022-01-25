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
    // ��� ������ ����Ʈ
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UListView* World_list_view;

    // ���� �κ��丮 ������ ����Ʈ
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UListView* Inventory_list_view;
};
