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

    Chaos::Pair<UTextBlock*, FString> arrTextPair[]
    {
        Chaos::MakePair<UTextBlock*, FString>(NameTxt, SlotItemData.Name),
        Chaos::MakePair<UTextBlock*, FString>(CategoryTxt, SlotItemData.Category),
        Chaos::MakePair<UTextBlock*, FString>(DescriptionTxt, SlotItemData.Description),
        Chaos::MakePair<UTextBlock*, FString>(CapacityTxt, (SlotItemData.Capacity == 0) ? "" : FString::FromInt(SlotItemData.Capacity))
    };
    for (auto item : arrTextPair)
    {
        if (item.First)
            item.First->SetText(FText::FromString(item.Second));
    }
}