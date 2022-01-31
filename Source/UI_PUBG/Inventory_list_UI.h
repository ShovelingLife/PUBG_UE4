#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/SizeBox.h"
#include "Inventory_list_UI.generated.h"

DECLARE_DELEGATE(FDele_set_weapon_slot_null)

UCLASS()
class UI_PUBG_API UInventory_list_UI : public UUserWidget
{
	GENERATED_BODY()

private:
    class AUI_manager* mp_UI_manager                   = nullptr;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*  World_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView* World_list_view;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*  Inventory_list_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UListView* Inventory_list_view;

    FDele_set_weapon_slot_null dele_set_weapon_slot_null;

protected:
    virtual void NativeTick(const FGeometry&, float) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;

    virtual FReply NativeOnMouseButtonUp(const FGeometry&, const FPointerEvent&) override;

    virtual void NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _pointer_event, class UDragDropOperation*& _operation) override;

    virtual bool NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, class UDragDropOperation* _operation) override;
};