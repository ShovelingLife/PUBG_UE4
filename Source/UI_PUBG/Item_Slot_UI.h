#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Item_Slot_UI.generated.h" 

DECLARE_DELEGATE_OneParam(FDele_check_for_slot, UObject*)

class UImage;
class UTextBlock;
class USizeBox;
class UBorder;
class UHorizontalBox;

USTRUCT()
struct Fs_slot_item_data
{
    GENERATED_BODY()

public:
    FString name = "";
    int     image_index = 0;
    int     count = 0;

public:
    Fs_slot_item_data() = default;

    // 아이템 명칭/개수/이미지 인덱스(UI매니저)
    Fs_slot_item_data(FString _name, int _image_index, int _count = 1) : name(_name), image_index(_image_index), count(_count) { }
};

UCLASS()
class UI_PUBG_API UItem_Slot_UI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UHorizontalBox* Main_horizontal_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*       Background_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*        Item_border;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*         Item_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*     Name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*     Count_txt;

    FDele_check_for_slot dele_check_for_slot;
    Fs_slot_item_data    item_data;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    // 아이템 설정 시
    virtual void NativeOnListItemObjectSet(UObject*);

public:
    void Set_as_cursor(FVector2D);
};