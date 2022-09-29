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
    UPROPERTY(EditAnywhere, meta = (BindWidget)) UItemSlotUI* CurrentItemSlot = nullptr;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCharacterSlotUI*       CharacterSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventoryListUI*       InventoryListUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventoryWeaponSlotUI* InventoryWeaponSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTooltipUI*             TooltipUI;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
    void CheckTooltipMouseDistance();

public:
    UFUNCTION() void SetTooltipVisibility(UItemSlotUI* pItemSlotUI, ESlateVisibility TooltipVisibility);

    UFUNCTION() void MoveSlotCursor(FVector2D NewPos);
};