#include "TooltipUI.h"
#include "ItemSlotUI.h"
#include "UI_manager.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UTooltipUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

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

void UTooltipUI::SetData(FsSlotItemData SlotItemData)
{
    // 데이터 설정
    ItemImg->SetBrushFromTexture(AUI_manager::GetTexture2D(SlotItemData));

    TArray<TPair<UTextBlock*, FString>> arrTextPair
    {
        TPair<UTextBlock*, FString>{ NameTxt,        SlotItemData.Name },
        TPair<UTextBlock*, FString>{ CategoryTxt,    SlotItemData.Category },
        TPair<UTextBlock*, FString>{ DescriptionTxt, SlotItemData.Description },
        TPair<UTextBlock*, FString>{ CapacityTxt,   (SlotItemData.Capacity == 0) ? "" : FString::FromInt(SlotItemData.Capacity) }
    };
    for (auto item : arrTextPair)
    {        
        if (auto p_textBlock = item.Key)
            p_textBlock->SetText(FText::FromString(item.Value));
    }
}