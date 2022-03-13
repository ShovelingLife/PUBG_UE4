#include "InventoryListUI.h"
#include "ItemSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "Characters/CustomPlayer.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Player_weapons/WeaponManager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UInventoryListUI::NativeConstruct()
{
    Super::NativeConstruct();
    GetItemListWidth();

    // 인벤토리에 아이템 추가되는 델리게이트 설정
    if (auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
        p_customGameInst->DeleSetItemOntoInventory.BindUFunction(this, "SetItemOntoInventory");

}

void UInventoryListUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);

    if (!mpWeaponManager)
        mpWeaponManager = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetWeaponManager();
}

FReply UInventoryListUI::NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseButtonDown(_InGeometry, _InMouseEvent);
    
    // 왼쪽 클릭 / 마우스 커서 변경
    if (_InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        HighlightImg->SetVisibility(ESlateVisibility::Hidden);

        // 클릭시 툴팁 숨김
        auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

        if (!subGameInst)
            subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(mpSlotObj, ESlateVisibility::Hidden);
    }
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(_InMouseEvent, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UInventoryListUI::NativeOnMouseMove(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseMove(_InGeometry, _InMouseEvent);

    // 움직일 때마다 현재 이미지의 위치를 구함    
    auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (!subGameInst)
        return FReply::Unhandled();

    bool bFirst = false;
    auto distance = subGameInst->GetDistanceBetweenSlotCursor(mpSlotObj, bFirst);

    if (subGameInst->IsMouseLeftFromUI(distance, bFirst))
        HighlightImg->SetVisibility(ESlateVisibility::Hidden);

    return FReply::Handled();
}

void UInventoryListUI::NativeOnMouseLeave(const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseLeave(_InMouseEvent);
    HighlightImg->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryListUI::NativeOnDragDetected(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent, UDragDropOperation*& _OutOperation)
{
    Super::NativeOnDragDetected(_InGeometry, _InMouseEvent, _OutOperation);
    
    // 마우스 위치를 구함
    auto      p_slot    = CreateWidget<UItemSlotUI>(GetWorld(), BP_ItemSlotUI);
    FVector2D mousePos = _InGeometry.AbsoluteToLocal(_InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);
    
    if (!p_slot ||
        !mpSlotObj)
        return;

    // 슬롯 설정
    auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (subGameInst)
        subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(nullptr, ESlateVisibility::Hidden);

    p_slot->pDraggedItem = mpSlotObj->pDraggedItem;
    p_slot->ItemData     = mpSlotObj->ItemData;
    p_slot->Priority     = 1;
    p_slot->DeleSetSlotNull.BindUFunction(this, "DeleteFromList");
    p_slot->DeleSwapWeaponSlot.BindUFunction(this, "SwapWeaponSlot");
    p_slot->SetAsCursor(mousePos);

    // 드래그 구현
    auto p_dragOperation = NewObject<UCustomDragDropOperation>();
    p_dragOperation->pSlotUI           = p_slot;
    p_dragOperation->DefaultDragVisual = p_slot;
    p_dragOperation->Pivot             = EDragPivot::MouseDown;
    p_dragOperation->ItemData          = mpSlotObj->ItemData;
    p_dragOperation->bFromList = true;
    _OutOperation  = p_dragOperation;
}

bool UInventoryListUI::NativeOnDrop(const FGeometry& _InGeometry, const FDragDropEvent& _InMouseEvent, UDragDropOperation* _Operation)
{
    Super::NativeOnDrop(_InGeometry, _InMouseEvent, _Operation);
    auto p_dragOperation = Cast<UCustomDragDropOperation>(_Operation);
    auto p_slot          = p_dragOperation->pSlotUI;

    if (!p_slot)
        return false;

    // 같은 아이템일 시
    if (mpSlotObj &&
        p_slot->pDraggedItem == mpSlotObj->pDraggedItem)
        return false;

    //p_slot->bShowTooltip = false;

    // 마우스 위치 구하기
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 월드 리스트에 드롭
    if (mousePos.X > 100.f &&
        mousePos.X < 325.f)
    {
        p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
        p_slot->DeleSetSlotNull.ExecuteIfBound();
        WorldListView->AddItem(p_slot);
        mpWeaponManager->Drop(mpWeaponManager->GetWeaponIndex(p_slot->pDraggedItem));
    }
    // 인벤토리 리스트에 드롭
    else
    {
        if (p_dragOperation->bGun)
            return false;

        p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
        p_slot->DeleSetSlotNull.ExecuteIfBound();
        InventoryListView->AddItem(p_slot);
    }
    return true;
}

void UInventoryListUI::GetItemListWidth()
{
    // 월드 사이즈 박스 넓이 구함
    if (auto p_worldListCanvasSlot = Cast<UCanvasPanelSlot>(WorldListSizeBox->Slot))
    {
        FVector2D worldSizeBoxPos  = p_worldListCanvasSlot->GetPosition();
        FVector2D worldSizeBoxSize = p_worldListCanvasSlot->GetSize();
        mWorldSizeBoxWidth         = worldSizeBoxPos.X + worldSizeBoxSize.X;
    }
    // 인벤토리 사이즈 박스 넓이 구함    
    if (auto p_inventoryListCanvasSlot = Cast<UCanvasPanelSlot>(InventoryListSizeBox->Slot))
    {
        FVector2D inventorySizeBoxPos  = p_inventoryListCanvasSlot->GetPosition();
        FVector2D inventorySizeBoxSize = p_inventoryListCanvasSlot->GetSize();
        mInventorySizeBoxWidth         = inventorySizeBoxPos.X + inventorySizeBoxSize.X;
    }
}

// this가 넘어오므로 널 체크 불필요
void UInventoryListUI::CheckForHoveredItem(UItemSlotUI* _pSlotObj)
{
    // 현재 이미지 위치를 구함
    FVector2D movePos = FVector2D::ZeroVector, dummy_vec;
    auto      cachedGeometry = _pSlotObj->GetCachedGeometry();
    mpSlotObj = _pSlotObj;
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cachedGeometry, FVector2D::ZeroVector, dummy_vec, movePos);
    movePos.X = 0.f;

    if (FMath::FloorToInt(movePos.Y) > 91.f)
        movePos.Y += 5.f;

    else
        movePos.Y = 86.f;

    if (auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(HighlightImg->Slot))
        p_canvasPanelSlot->SetPosition(movePos);

    HighlightImg->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryListUI::DeleteFromList()
{
    if (!mpSlotObj)
        return;

    // 월드 리스트부터 순차적으로 검색
    for (int i = 0; i < WorldListView->GetNumItems(); i++)
    {
        if (auto p_slot = Cast<UItemSlotUI>(WorldListView->GetItemAt(i)))
        {
            // 발견 시 해당하는 아이템 삭제
            if (p_slot->pDraggedItem == mpSlotObj->pDraggedItem)
            {
                WorldListView->RemoveItem(p_slot);
                mpSlotObj = nullptr;
                return;
            }
        }
    }
    // 인벤토리 리스트부터 순차적으로 검색
    for (int i = 0; i < InventoryListView->GetNumItems(); i++)
    {
        if (auto p_slot = Cast<UItemSlotUI>(InventoryListView->GetItemAt(i)))
        {
            // 발견 시 해당하는 아이템 삭제
            if (p_slot->pDraggedItem == mpSlotObj->pDraggedItem)
            {
                InventoryListView->RemoveItem(p_slot);
                mpSlotObj = nullptr;
                return;
            }
        }
    }
}

void UInventoryListUI::SwapWeaponSlot(UItemSlotUI* _pWeaponSlot)
{
    DeleteFromList();
    _pWeaponSlot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
    WorldListView->AddItem(_pWeaponSlot);
}

void UInventoryListUI::SetItemOntoInventory(ABaseInteraction* _pWeapon)
{
    if (!_pWeapon ||
        !mpWeaponManager)
        return;

    UItemSlotUI* p_slot = NewObject<UItemSlotUI>();

    int imageIndex = mpWeaponManager->GetWeaponType(_pWeapon);

    if (imageIndex == -1) // 예외 처리
        return;

    FsSlotItemData slotItemData(_pWeapon->ObjectGroupType, _pWeapon->ObjectType, imageIndex);
    p_slot->ItemData     = slotItemData;
    p_slot->pDraggedItem = _pWeapon;
    p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
    p_slot->DeleSetSlotNull.BindUFunction(this, "DeleteFromList");
    p_slot->DeleSwapWeaponSlot.BindUFunction(this, "SwapWeaponSlot");
    InventoryListView->AddItem(p_slot);
}