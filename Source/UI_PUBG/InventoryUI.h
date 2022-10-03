#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Events.h"
#include "Input/Reply.h"
#include "InventoryUI.generated.h"

class AUI_manager;
class UInventoryListUI;
class UCharacterSlotUI;
class UInventoryWeaponSlotUI;
class UItemSlotUI;
class UTooltipUI;

UCLASS()
class UI_PUBG_API UInventoryUI : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCharacterSlotUI*       CharacterSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventoryListUI*       InventoryListUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventoryWeaponSlotUI* InventoryWeaponSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTooltipUI*             TooltipUI;

public:
    UInventoryUI(const FObjectInitializer& Initializer);

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;


    FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


private:
    void CheckTooltipMouseDistance();

    void MoveSlotCursor();

public:
    UFUNCTION() void SetTooltipVisibility(UItemSlotUI* pItemSlotUI, ESlateVisibility TooltipVisibility);
};