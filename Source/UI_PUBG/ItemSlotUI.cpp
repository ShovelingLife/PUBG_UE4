﻿#include "ItemSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/Global.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
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

void UItemSlotUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

    // 현재 관련된 UI에 적용
    ItemImg->SetBrushFromTexture(AUI_manager::GetTexture2D(ItemData));
    NameTxt->SetText(FText::FromString(ItemData.Name));

    if (ItemData.Count > 0)
        CountTxt->SetText(FText::FromString(FString::FromInt(ItemData.Count)));

    if (this->IsHovered())
    {
        DeleCheckForSlot.ExecuteIfBound(this);

        if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
            subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(this, Visible);
    }
}

void UItemSlotUI::NativeOnListItemObjectSet(UObject* pObj)
{
    if (auto p_slot = Cast<UItemSlotUI>(pObj))
    {
        // 현재 변수들에 데이터 적용
        pDraggedItem        = p_slot->pDraggedItem;
        DeleCheckForSlot    = p_slot->DeleCheckForSlot;
        DeleSwapWeaponSlot  = p_slot->DeleSwapWeaponSlot;
        DeleDeleteFromList  = p_slot->DeleDeleteFromList;
        ItemData            = p_slot->ItemData;
    }
}

void UItemSlotUI::SetForAttachmentUI()
{
    NameTxt->RemoveFromParent();
    CountTxt->RemoveFromParent();
    BackgroundSizeBox->RemoveFromParent();
    MainCanvasPanel->AddChildToCanvas(ItemBorder);
}

void UItemSlotUI::SetAsCursor(FVector2D Pos)
{
    auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(MainHorizontalBox->Slot);
    
    if (!p_canvasPanelSlot)
        return;

    p_canvasPanelSlot->SetPosition(Pos);
    this->SetVisibility(Visible);
    ItemImg->SetBrushFromTexture(AUI_manager::GetTexture2D(ItemData));
    NameTxt->SetVisibility(Hidden);
    CountTxt->SetVisibility(Hidden);
    BackgroundSizeBox->SetVisibility(Hidden);
}