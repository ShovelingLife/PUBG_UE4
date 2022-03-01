#include "TooltipUI.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UTooltipUI::NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)
{
    Super::NativeTick(_MyGeometry, _InDeltaTime);

    if (this->GetVisibility() == ESlateVisibility::Visible)
    {
        if (auto p_CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot))
        {
            p_CanvasSlot->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()));
            /*FAnchors slotAnchor;
            slotAnchor.
            p_CanvasSlot->Ancho*/
        }
    }
}