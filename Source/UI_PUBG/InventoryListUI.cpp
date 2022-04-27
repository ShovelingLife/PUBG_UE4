#include "InventoryListUI.h"
#include "ItemSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Player_weapons/CoreThrowableWeapon.h"
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

    pGameInstanceSubsystemUI = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();
}

void UInventoryListUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
}

FReply UInventoryListUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    // 왼쪽 클릭 / 마우스 커서 변경
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        HighlightImg->SetVisibility(ESlateVisibility::Hidden);

        // 클릭시 툴팁 숨김
        if (pGameInstanceSubsystemUI)
            pGameInstanceSubsystemUI->DeleSetTooltipVisibility.ExecuteIfBound(mpSlotObj, ESlateVisibility::Hidden);
    }
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UInventoryListUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);

    // 움직일 때마다 현재 이미지의 위치를 구함    
    if (!pGameInstanceSubsystemUI)
        return FReply::Unhandled();

    bool b_first = false;
    auto distance = pGameInstanceSubsystemUI->GetDistanceBetweenSlotCursor(mpSlotObj, b_first);

    if (pGameInstanceSubsystemUI->IsMouseLeftFromUI(distance, b_first))
        HighlightImg->SetVisibility(ESlateVisibility::Hidden);

    return FReply::Handled();
}

void UInventoryListUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    HighlightImg->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryListUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    
    // 마우스 위치를 구함
    auto      p_slot   = CreateWidget<UItemSlotUI>(GetWorld(), BP_ItemSlotUI);
    FVector2D mousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);
    
    if (!p_slot    ||
        !mpSlotObj ||
        !pGameInstanceSubsystemUI)
        return;

    // 슬롯 설정    
    pGameInstanceSubsystemUI->DeleSetTooltipVisibility.ExecuteIfBound(nullptr, ESlateVisibility::Hidden);

    p_slot->pDraggedItem = mpSlotObj->pDraggedItem;
    p_slot->ItemData     = mpSlotObj->ItemData;
    p_slot->Priority     = 1;
    p_slot->DeleDeleteFromList.BindUFunction(this, "DeleteFromList");
    p_slot->DeleSwapWeaponSlot.BindUFunction(this, "SwapWeaponSlot");
    p_slot->DeleSwapInventoryExplosive.BindUFunction(this, "SwapInventoryExplosive");
    p_slot->DeleChangeItemCount.BindUFunction(this, "ChangeItemCount");
    p_slot->SetAsCursor(mousePos);

    // 드래그 구현
    auto p_dragOperation = NewObject<UCustomDragDropOperation>();
    p_dragOperation->pSlotUI           = p_slot;
    p_dragOperation->DefaultDragVisual = p_slot;
    p_dragOperation->Pivot             = EDragPivot::MouseDown;
    p_dragOperation->bFromInventoryList = true;
    OutOperation  = p_dragOperation;
}

bool UInventoryListUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InMouseEvent, UDragDropOperation* Operation)
{
    Super::NativeOnDrop(InGeometry, InMouseEvent, Operation);
    auto p_dragOperation = Cast<UCustomDragDropOperation>(Operation);
    auto p_slot          = p_dragOperation->pSlotUI;

    if (!p_slot ||
        !mpSlotObj)
        return false;

    // 같은 아이템일 시
    if (p_slot->pDraggedItem == mpSlotObj->pDraggedItem)
        return false;

    // 마우스 위치 구하기
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 월드 리스트에 드롭
    if (mousePos.X > 100.f &&
        mousePos.X < 325.f)
    {
        if (!pGameInstanceSubsystemUI)
            return false;

        p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
        p_slot->DeleDeleteFromList.ExecuteIfBound();
        WorldListView->AddItem(p_slot);

        if (auto p_weaponManager = pGameInstanceSubsystemUI->GetWeaponManager())
            p_weaponManager->Drop(p_weaponManager->GetWeaponIndex(p_slot->pDraggedItem));
    }
    // 인벤토리 리스트에 드롭
    else
    {
        if (p_dragOperation->bGun)
            return false;

        p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
        p_slot->DeleDeleteFromList.ExecuteIfBound();
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

UItemSlotUI* UInventoryListUI::GetInitializedSlotUI(ABaseInteraction* pWeapon, FsSlotItemData ItemData)
{
    auto p_slot = NewObject<UItemSlotUI>();
    p_slot->ItemData = ItemData;
    p_slot->pDraggedItem = pWeapon;
    p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
    p_slot->DeleDeleteFromList.BindUFunction(this, "DeleteFromList");
    p_slot->DeleSwapWeaponSlot.BindUFunction(this, "SwapWeaponSlot");
    return p_slot;
}

UItemSlotUI* UInventoryListUI::GetMatchingItemFromList(FString ItemName)
{
    for (auto item : InventoryListView->GetListItems())
    {
        if (auto p_slot = Cast<UItemSlotUI>(item))
        {
            if (p_slot->ItemData.Name == ItemName)
                return p_slot;
        }
    }
    return nullptr;
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

// this가 넘어오므로 널 체크 불필요
void UInventoryListUI::CheckForHoveredItem(UItemSlotUI* pSlotObj)
{
    // 현재 이미지 위치를 구함
    FVector2D movePos = FVector2D::ZeroVector, dummy_vec;
    auto      cachedGeometry = pSlotObj->GetCachedGeometry();
    mpSlotObj = pSlotObj;
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cachedGeometry, FVector2D::ZeroVector, dummy_vec, movePos);

    // 리스트 판별
    if (movePos.X == 100.f)
    {
        movePos.X = 0.f;
        movePos.Y = ((int)movePos.Y > 86) ? (movePos.Y + 5.f) : 86.f;
    }
    else
    {
        movePos.X = 255.f;
        movePos.Y = ((int)movePos.Y > 82) ? (movePos.Y + 5.f) : 82.f;
    }
    if (auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(HighlightImg->Slot))
        p_canvasPanelSlot->SetPosition(movePos);

    HighlightImg->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryListUI::SwapWeaponSlot(UItemSlotUI* pWeaponSlot)
{
    DeleteFromList();
    pWeaponSlot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
    WorldListView->AddItem(pWeaponSlot);
}

void UInventoryListUI::ChangeItemCount(UItemSlotUI* pSlotObj)
{
    if (!pSlotObj)
        return;

    // 차감 후 대입
    if (auto p_slot = GetMatchingItemFromList(pSlotObj->ItemData.Name))
    {
        auto tmpSlot = GetInitializedSlotUI(pSlotObj->pDraggedItem, pSlotObj->ItemData);
        InventoryListView->RemoveItem(p_slot);
        InventoryListView->AddItem(tmpSlot);
    }
}

void UInventoryListUI::SetItemOntoInventory(ABaseInteraction* pWeapon, bool bDeleteFromList /* = false */)
{
    if (!pGameInstanceSubsystemUI)
        return;

    if (bDeleteFromList)
        DeleteFromList();

    FsSlotItemData itemData = FsSlotItemData::GetDataFrom(pWeapon);

    if (itemData.IsEmpty())
        return;

    // 현재 아이템이 있는지 확인
    UItemSlotUI* p_existingSlot = GetMatchingItemFromList(itemData.Name);

    // 아이템이 존재함
    if (p_existingSlot)
    {
        p_existingSlot->ItemData.Count++;
        InventoryListView->RemoveItem(p_existingSlot);
    }
    InventoryListView->AddItem((p_existingSlot) ? p_existingSlot : GetInitializedSlotUI(pWeapon, itemData));
}

void UInventoryListUI::SwapInventoryExplosive(ACoreThrowableWeapon* NewExplosive, ACoreThrowableWeapon* OldExplosive)
{
    if (!NewExplosive ||
        !OldExplosive)
        return;

    // 인벤토리 리스트부터 순차적으로 검색
    for (int i = 0; i < InventoryListView->GetNumItems(); i++)
    {
        if (auto p_slot = Cast<UItemSlotUI>(InventoryListView->GetItemAt(i)))
        {
            // 발견 시 해당하는 아이템 삭제
            if (p_slot->ItemData.Name == NewExplosive->WeaponData.Type)
            {
                // 장착 중인 무기가 인벤토리에 1개 이상 있음
                if (p_slot->ItemData.Count > 0)
                {
                    p_slot->ItemData.Count++;
                    return;
                }
            }
            InventoryListView->RemoveItem(p_slot);
            //SetItemOntoInventory(Cast<ABaseInteraction>(NewExplosive));
            return;
        }
    }
}