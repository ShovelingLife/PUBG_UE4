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

private:
    AUI_manager* mpUI_manager;
    UItemSlotUI* mpCurrentItemSlot;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCharacterSlotUI*       CharacterSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventoryListUI*       InventoryListUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UInventoryWeaponSlotUI* InventoryWeaponSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTooltipUI*             TooltipUI;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

private:

    void CheckTooltipMouseDistance();

public:
    UFUNCTION()
    void SetTooltipVisibility(UItemSlotUI* _pItemSlotUI, ESlateVisibility _Visibility);
};