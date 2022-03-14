﻿#include "ItemSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/PanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UItemSlotUI::NativeConstruct()
{
    Super::NativeConstruct();    
}

void UItemSlotUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);
    
    if (this->IsHovered())
    {
        DeleCheckForSlot.ExecuteIfBound(this);

        if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
            subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(this, ESlateVisibility::Visible);
    }
}

void UItemSlotUI::NativeOnListItemObjectSet(UObject* _pObj)
{
    auto p_slot = Cast<UItemSlotUI>(_pObj);

    if (p_slot)
    {
        ItemImg->SetBrushFromTexture(AUI_manager::GetTexture2D(p_slot->ItemData));
        NameTxt->SetText(FText::FromString(p_slot->ItemData.Name));
        pDraggedItem       = p_slot->pDraggedItem;
        DeleCheckForSlot   = p_slot->DeleCheckForSlot;
        DeleSwapWeaponSlot = p_slot->DeleSwapWeaponSlot;
        DeleSetSlotNull    = p_slot->DeleSetSlotNull;
        ItemData           = p_slot->ItemData;

        if (ItemData.Count > 1)
            CountTxt->SetText(FText::FromString(FString::FromInt(p_slot->ItemData.Count)));
    }
}

void UItemSlotUI::SetAsCursor(FVector2D _Pos)
{
    auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(MainHorizontalBox->Slot);
    
    if (p_canvasPanelSlot)
        p_canvasPanelSlot->SetPosition(_Pos);

    this->SetVisibility(ESlateVisibility::Visible);
    ItemImg->SetBrushFromTexture(AUI_manager::GetTexture2D(ItemData));
    NameTxt->SetVisibility(ESlateVisibility::Hidden);
    CountTxt->SetVisibility(ESlateVisibility::Hidden);
    BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
}