#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_list_UI.generated.h"

DECLARE_DELEGATE(FDele_set_weapon_slot_null)

class UImage;
class UListView;
class USizeBox;
class UTextBlock;
class UItem_Slot_UI;

UCLASS()
class UI_PUBG_API UInventory_list_UI : public UUserWidget
{
	GENERATED_BODY()

private:
    UPROPERTY() UItem_Slot_UI* p_slot_obj;
    float m_world_size_box_width = 0.f;
    float m_inventory_size_box_width = 0.f;

public:
    // 월드 리스트 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* World_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*   World_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView*  World_list_view;

    // 인벤토리 리스트 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Inventory_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*   Inventory_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView*  Inventory_list_view;

    // 기타 UI 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*    Highlight_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*    Separator_img;

    UPROPERTY(EditDefaultsOnly, Category = Item_slot) TSubclassOf<UItem_Slot_UI> p_item_slot_UI_class;
    FDele_set_weapon_slot_null dele_set_weapon_slot_null;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;

    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnMouseLeave(const FPointerEvent&) override;

    virtual void NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _pointer_event, class UDragDropOperation*& _operation) override;

    virtual bool NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, class UDragDropOperation* _operation) override;

private:
    void Get_item_list_width();

    FVector2D Get_distance_between_slot_cursor();

public:
    UFUNCTION()
    void Check_for_hovered_item(UItem_Slot_UI* _p_slot_obj);
};