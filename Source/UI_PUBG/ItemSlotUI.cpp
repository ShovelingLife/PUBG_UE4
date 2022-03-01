#include "ItemSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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
    UpdateTooltipVisibility();

    if (this->IsHovered())
        DeleCheckForSlot.ExecuteIfBound(this);
}

void UItemSlotUI::NativeOnListItemObjectSet(UObject* _pObj)
{
    auto p_slot = Cast<UItemSlotUI>(_pObj);

    if (p_slot)
    {
        ItemImg->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::MapInventoryWeaponTex[p_slot->ItemData.ImageIndex]));
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

void UItemSlotUI::UpdateTooltipVisibility()
{
    FVector2D distance = GetDistanceBetweenSlotCursor();
    auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (mbShouldCheckTooltipUI)
    {
        if (distance.X <= 0.1f  ||
            distance.X >= 225.f ||
            distance.Y <= 0.1f  ||
            distance.Y >= 50.f)
        {
            if (subGameInst)
                subGameInst->DeleHideTooltip.ExecuteIfBound(ESlateVisibility::Hidden);

            mbShouldCheckTooltipUI = false;
        }
        return;
    }   
    if (subGameInst)
        subGameInst->DeleHideTooltip.ExecuteIfBound(ESlateVisibility::Visible);

    mbShouldCheckTooltipUI = true;
    //GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Cyan, FString("Current distance : ").Append(distance.ToString()));
}

void UItemSlotUI::SetAsCursor(FVector2D _Pos)
{
    auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(MainHorizontalBox->Slot);
    
    if (p_canvasPanelSlot)
        p_canvasPanelSlot->SetPosition(_Pos);

    this->SetVisibility(ESlateVisibility::Visible);
    ItemImg->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::MapInventoryWeaponTex[ItemData.ImageIndex]));
    NameTxt->SetVisibility(ESlateVisibility::Hidden);
    CountTxt->SetVisibility(ESlateVisibility::Hidden);
    BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

FVector2D UItemSlotUI::GetDistanceBetweenSlotCursor()
{
    // 마우스 위치를 구함   
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 현재 이미지 위치를 구함
    FVector2D imgPos = FVector2D::ZeroVector, dummy_vec;
    auto      cachedGeometry = this->GetCachedGeometry();
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cachedGeometry, FVector2D::ZeroVector, dummy_vec, imgPos);

    // 마우스 좌표 - 이미지 좌표 간 거리
    float posX = (imgPos.X > mousePos.X) ? imgPos.X - mousePos.X : mousePos.X - imgPos.X,
          posY = (imgPos.Y > mousePos.Y) ? imgPos.Y - mousePos.Y : mousePos.Y - imgPos.Y;

    return FVector2D(posX, posY);
}