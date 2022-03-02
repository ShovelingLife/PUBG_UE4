#include "TooltipUI.h"
#include "ItemSlotUI.h"
#include "UI_manager.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UTooltipUI::NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)
{
    Super::NativeTick(_MyGeometry, _InDeltaTime);

    if (this->GetVisibility() == ESlateVisibility::Visible)
    {
        if (auto p_CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot))
        {
            auto mouseViewportPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
            mouseViewportPos += FVector2D(15.f);
            p_CanvasSlot->SetPosition(mouseViewportPos);
        }
    }
}

void UTooltipUI::SetData(UItemSlotUI* _pItemSlotUI)
{
    if (!_pItemSlotUI)
        return;

    // 데이터 설정
    auto data = _pItemSlotUI->ItemData;
    NameTxt->SetText(FText::FromString(data.Name));
    ItemImg->SetBrushFromTexture(Cast<UTexture2D>(AUI_manager::MapInventoryWeaponTex[data.ImageIndex]));
}