#include "ItemSlotUI.h"
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

    if (this->IsHovered())
    {
        GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::FromInt(ItemData.Count));
        DeleCheckForSlot.ExecuteIfBound(this);

        if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
            subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(this, Visible);
    }
}

void UItemSlotUI::NativeOnListItemObjectSet(UObject* pObj)
{
    // 한번만 호출됨, CreateWidget 함수로 생성한 후 ListView에서 AddItem 할 시 
    if (auto p_slot = Cast<UItemSlotUI>(pObj))
    {
        // 현재 변수들에 데이터 적용
        GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Cyan, "ObjectSet");
        pDraggedItem       = p_slot->pDraggedItem;
        DeleCheckForSlot   = p_slot->DeleCheckForSlot;
        DeleSwapWeaponSlot = p_slot->DeleSwapWeaponSlot;
        DeleDeleteFromList = p_slot->DeleDeleteFromList;
        ItemData           = p_slot->ItemData;

        // 현재 관련된 UI에 적용
        ItemImg->SetBrushFromTexture(AUI_manager::GetTexture2D(ItemData));
        NameTxt->SetText(FText::FromString(ItemData.Name));
        CountTxt->SetText(FText::FromString(FString::FromInt(ItemData.Count)));
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