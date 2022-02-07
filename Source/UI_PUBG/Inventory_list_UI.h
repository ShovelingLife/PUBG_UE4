#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_list_UI.generated.h"

DECLARE_DELEGATE(FDele_set_weapon_slot_null)

class USizeBox;
class UListView;
class UImage;
class UItem_Slot_UI;

UCLASS()
class UI_PUBG_API UInventory_list_UI : public UUserWidget
{
	GENERATED_BODY()

    enum class e_hovered_list_type
    {
        WORLD,
        INVENTORY,
        NONE
    };

private:
    class AUI_manager*  mp_UI_manager = nullptr;
    UPROPERTY() TWeakObjectPtr<UItem_Slot_UI> wk_p_slot_obj;
    e_hovered_list_type m_hovered_list_type = e_hovered_list_type::NONE;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*  World_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView* World_list_view;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*  Inventory_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView* Inventory_list_view;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*    Highlight_img;

    FDele_set_weapon_slot_null dele_set_weapon_slot_null;

protected:
    virtual void NativeTick(const FGeometry&, float) override;    

    virtual bool NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, class UDragDropOperation* _operation) override;

private:
    void Change_highlight_img_pos();

public:
    UFUNCTION()
    void Check_for_hovered_item(UItem_Slot_UI* _p_slot_obj);
};