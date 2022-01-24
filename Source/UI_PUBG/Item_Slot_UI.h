#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Global.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Item_Slot_UI.generated.h"

UCLASS()
class UI_PUBG_API UItem_Slot_UI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Item_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Name_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Count_txt;

    Fs_slot_item_data item_data;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    // 아이템 설정 시
    virtual void NativeOnListItemObjectSet(UObject*);

    // 아이템 선택 시
    virtual void NativeOnItemSelectionChanged(bool) override;

public:
    void Set_slot_item_data(Fs_slot_item_data);
};