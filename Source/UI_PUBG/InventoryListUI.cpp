#include "InventoryListUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "InventoryManager.h"
#include "ItemSlotUI.h"
#include "SlotItemData.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Farmable_items/CoreAttachment.h"
#include "Farmable_items/CoreBackpack.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/Global.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/Progressbar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UInventoryListUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitButtons();
    GetItemListWidth();

    // 인벤토리에 아이템 추가되는 함수 바인딩
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleSetItemOntoInventory.BindUFunction(this, "UpdateInventoryList");

    pGameInstanceSubsystemUI = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();
}

void UInventoryListUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    auto total = (mMaxCapacity / CurCapacity) * 0.1f;
    auto percent = (total > 1.f) ? (mMaxCapacity / CurCapacity) * 0.01f : total;
    // 갱신
    BackpackFreeSpaceBar->SetPercent((CurCapacity > 0) ? percent : 0.f);
    //CheckInventoryCapacity();
}

FReply UInventoryListUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    // 왼쪽 클릭 / 마우스 커서 변경
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        ImgHighlight->SetVisibility(ESlateVisibility::Hidden);

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
        ImgHighlight->SetVisibility(ESlateVisibility::Hidden);

    return FReply::Handled();
}

void UInventoryListUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    ImgHighlight->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryListUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    
    // 마우스 위치를 구함
    FVector2D mousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);
    
    if (!mpSlotObj ||
        !pGameInstanceSubsystemUI)
        return;

    // 데이터를 옮김
    //auto data = FsSlotItemData::GetDataFrom(mpSlotObj->pDraggedItem);
    //auto p_tmpData = &data;
    //p_tmpData->Name = mpSlotObj->ItemData.Name;
    auto p_slot = GetInitializedSlotUI(mpSlotObj->pDraggedItem);

    if (!p_slot)
        return;

    // 슬롯 설정    
    pGameInstanceSubsystemUI->DeleSetTooltipVisibility.ExecuteIfBound(nullptr, ESlateVisibility::Hidden);                                           
    p_slot->SetAsCursor(mousePos);

    // 무기 부속품일 시 해당되는 칸 설정
    /*if (p_slot->ItemData.Category == "Attachment")
        pGameInstanceSubsystemUI->DeleVerifyAttachmentSlot.ExecuteIfBound(Cast<ACoreAttachment>(p_slot->pDraggedItem));*/

    // 드래그 구현
    auto p_dragOperation = NewObject<UCustomDragDropOperation>();
    p_dragOperation->Init(p_slot, "Inventory");
    OutOperation  = p_dragOperation;
}

void UInventoryListUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

    if (!pGameInstanceSubsystemUI)
        return;

    if (auto p_customDragOp = Cast<UCustomDragDropOperation>(InOperation))
    {
        if (auto p_slot = p_customDragOp->pSlotUI)
            pGameInstanceSubsystemUI->DeleVerifyAttachmentSlot.ExecuteIfBound(nullptr);
    }
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
        LV_WorldList->AddItem(p_slot);

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
        LV_InventoryList->AddItem(p_slot);
    }
    return true;
}

void UInventoryListUI::InitButtons()
{
    BtnOrderAlphabet->SetClickMethod(EButtonClickMethod::MouseDown);
    BtnOrderAlphabet->OnClicked.AddDynamic(this, &UInventoryListUI::OrderAlphabetically);
    BtnOrderRecent->SetClickMethod(EButtonClickMethod::MouseDown);
    BtnOrderRecent->OnClicked.AddDynamic(this, &UInventoryListUI::OrderRecently);
    TxtOrderAlphabet->SetColorAndOpacity(Global::WhiteColor);
    TxtOrderRecent->SetColorAndOpacity(Global::GrayColor);
}

void UInventoryListUI::OrderInventory(FString Type)
{
    // 색상 갱신
    auto whiteColor = Global::WhiteColor, grayColor = Global::GrayColor;
    TxtOrderAlphabet->SetColorAndOpacity((Type == "Alphabet") ? whiteColor : grayColor);
    TxtOrderRecent->SetColorAndOpacity((Type == "Recent") ? whiteColor : grayColor);

    // 인벤토리를 종류에 따라 정렬
    if (Type == "Alphabet")
    {

    }
    else if (Type == "Recent")
    {

    }
    /*for (auto  : index)
    {
    }*/
}

void UInventoryListUI::GetItemListWidth()
{
    // 월드 사이즈 박스 넓이 구함
    if (auto p_worldListCanvasSlot = Cast<UCanvasPanelSlot>(SB_WorldList->Slot))
    {
        FVector2D SB_WorldPos  = p_worldListCanvasSlot->GetPosition();
        FVector2D SB_worldSize = p_worldListCanvasSlot->GetSize();
        mSB_WidthWorld         = (SB_WorldPos + SB_worldSize).X;
    }
    // 인벤토리 사이즈 박스 넓이 구함    
    if (auto p_inventoryListCanvasSlot = Cast<UCanvasPanelSlot>(SB_InventoryList->Slot))
    {
        FVector2D SB_InvenPos  = p_inventoryListCanvasSlot->GetPosition();
        FVector2D SB_InvenSize = p_inventoryListCanvasSlot->GetSize();
        mSB_WidthInven         = (SB_InvenPos + SB_InvenSize).X;
    }
}

UItemSlotUI* UInventoryListUI::GetInitializedSlotUI(ABaseInteraction* pObj)
{
    auto p_slot = CreateWidget<UItemSlotUI>(this, BP_ItemSlotUI);
    p_slot->ItemData     = FsSlotItemData::GetDataFrom(pObj);
    p_slot->pDraggedItem = pObj;
    p_slot->Priority = 1;
    p_slot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
    p_slot->DeleDeleteFromList.BindUFunction(this, "DeleteFromList");
    p_slot->DeleSwapWeaponSlot.BindUFunction(this, "SwapWeaponSlot");
    p_slot->DeleSwapInventoryExplosive.BindUFunction(this, "SwapInventoryExplosive");
    return p_slot;
}

UItemSlotUI* UInventoryListUI::GetMatchingItemFromList(FString ItemName) const
{
    if (pGameInstanceSubsystemUI)
    {
        if (auto p_inventoryManager = pGameInstanceSubsystemUI->GetInventoryManager())
        {
            auto mapCurrentItems = p_inventoryManager->MapCurrentItems;

            if (auto p_item = mapCurrentItems.Find(ItemName))
                return *p_item;
        }
    }
    return nullptr;
}

void UInventoryListUI::DeleteFromList()
{
    UItemSlotUI* p_itemSlotUI = nullptr;
    bool b_found = false;

    if (!mpSlotObj)
        return;

    // 월드 리스트부터 순차적으로 검색
    for (int i = 0; i < LV_WorldList->GetNumItems(); i++)
    {
        if (auto p_slot = Cast<UItemSlotUI>(LV_WorldList->GetItemAt(i)))
        {
            // 발견 시 해당하는 아이템 삭제
            if (p_slot->pDraggedItem == mpSlotObj->pDraggedItem)
            {
                p_itemSlotUI = p_slot;
                LV_WorldList->RemoveItem(p_slot);
                mpSlotObj = nullptr;
                break;
            }
        }
    }
    // 인벤토리 리스트부터 순차적으로 검색
    for (int i = 0; i < LV_InventoryList->GetNumItems(); i++)
    {
        if (p_itemSlotUI)
            break;

        if (auto p_slot = Cast<UItemSlotUI>(LV_InventoryList->GetItemAt(i)))
        {
            // 발견 시 해당하는 아이템 삭제
            if (p_slot->pDraggedItem == mpSlotObj->pDraggedItem)
            {
                p_itemSlotUI = p_slot;
                LV_InventoryList->RemoveItem(p_slot);
                mpSlotObj = nullptr;
                break;
            }
        }
    }
    // 아이템 삭제
    if (pGameInstanceSubsystemUI &&
        p_itemSlotUI)
    {
        if (auto p_inventoryManager = pGameInstanceSubsystemUI->GetInventoryManager())
            p_inventoryManager->MapCurrentItems.Remove(p_itemSlotUI->ItemData.Category);
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
    //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, movePos.ToString());
    
    // 리스트 판별 월드 리스트
    if (movePos.X == 100.f)
    {        
        movePos.X = 0.f;
        //movePos.Y = (movePos.Y > 143.9f) ? (movePos.Y + 5.f) : 143.9f;
    }
    // 인벤토리 리스트
    else
    {
        movePos.X = 248.25f;
        movePos.Y = (movePos.Y > 149.f) ? (movePos.Y - 57.7f) : 86.5f;
    }
    if (auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(ImgHighlight->Slot))
        p_canvasPanelSlot->SetPosition(movePos);

    ImgHighlight->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryListUI::SwapWeaponSlot(UItemSlotUI* pWeaponSlot)
{
    DeleteFromList();
    pWeaponSlot->DeleCheckForSlot.BindUFunction(this, "CheckForHoveredItem");
    LV_WorldList->AddItem(pWeaponSlot);
}

void UInventoryListUI::ChangeItemCount(ABaseInteraction* pObj, bool bAdd /* = true*/)
{
    auto p_inventoryManager = pGameInstanceSubsystemUI->GetInventoryManager();

    // 새로 생성한 슬롯 UI
    auto p_tmpSlot = GetInitializedSlotUI(pObj);

    if (!p_inventoryManager ||
        !p_tmpSlot)
        return;

    auto itemData = p_tmpSlot->ItemData;
    auto itemName = itemData.Name;

    // 현재 아이템이 있는지 확인 및 아이템 개수 갱신
    auto mapCurrentItems = &p_inventoryManager->MapCurrentItems;

    // 비어있을 시 현재 장착 중인 총기 총알 종류를 가져옴
    auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    auto p_weaponManager = p_player->GetWeaponManager();
    
    // 발포 되었을 때만
    if (auto p_gun = p_weaponManager->GetCurrentGun())
    {
        if (p_gun->bShooting)
            itemName = p_gun->WeaponData.BulletType;
    }
    // 아이템 개수 확인
    if (bAdd &&
        CurCapacity >= mMaxCapacity)
    {
        // 팝업 UI 설정
        if (auto p_customGameInst = UCustomGameInstance::GetInst())
            p_customGameInst->DeleSetFadingTxt.ExecuteIfBound("Inventory limit reached");

        return;
    }
    // 현재 리스트에 존재하고 있는 슬롯 UI 아이템이 존재함
    if (auto p_slot = mapCurrentItems->FindRef(itemName))
    {
        // 기존 아이템 개수 추가 / 차감 후 적용
        auto& itemCount = p_slot->ItemData.Count;
        itemCount = (bAdd) ? itemCount + itemData.Count : --itemCount;
        p_tmpSlot->ItemData = p_slot->ItemData;

        // 기존 슬롯을 제거
        LV_InventoryList->RemoveItem(p_slot);

        // 총알 0개면 그대로 리스트에서 제거
        if (itemCount == 0)
            return;

        // 삭제 후 재추가 (위젯을 재생성함)
        LV_InventoryList->AddItem(p_tmpSlot);
        mapCurrentItems->Remove(itemName);
        mapCurrentItems->Add(itemName, p_tmpSlot);
        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::FromInt(mapCurrentItems->Num()));
    }
    else
    {
        // 빈 정보인지 재검사
        if (itemName != "")
        {
            LV_InventoryList->AddItem(p_tmpSlot);
            mapCurrentItems->Add(itemName, p_tmpSlot);
        }
    }
}

void UInventoryListUI::UpdateInventoryList(ABaseInteraction* pObj, bool bDeleteFromList /* = false */)
{
    if (!pObj)
        return;

    if (bDeleteFromList)
        DeleteFromList();

    // 장착 가능할 경우 바로 장착 / 장착 되어있을 시 교체
    if (pObj->IsA<ACoreBackpack>())
    {
        ACoreBackpack* backPack = Cast<ACoreBackpack>(pObj);
        mMaxCapacity = backPack->Data.PropVal;

        // 저장할 수 있는 한계치를 변경
        if (auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            p_customGameInst->DeleSetInventoryCapacity.ExecuteIfBound(mMaxCapacity);
    }
    else
        ChangeItemCount(pObj);
}

void UInventoryListUI::SwapInventoryExplosive(ACoreThrowableWeapon* NewExplosive, ACoreThrowableWeapon* OldExplosive)
{
    if (!NewExplosive ||
        !OldExplosive)
        return;

    // 인벤토리 리스트부터 순차적으로 검색
    for (int i = 0; i < LV_InventoryList->GetNumItems(); i++)
    {
        if (auto p_slot = Cast<UItemSlotUI>(LV_InventoryList->GetItemAt(i)))
        {
            auto itemData = p_slot->ItemData;
            // 발견 시 해당하는 아이템 삭제
            if (itemData.Name == NewExplosive->WeaponData.Type)
            {
                // 장착 중인 무기가 인벤토리에 1개 이상 있음
                if (itemData.Count > 0)
                {
                    p_slot->ItemData.Count++;
                    return;
                }
            }
            LV_InventoryList->RemoveItem(p_slot);
            //SetItemOntoInventory(Cast<ABaseInteraction>(NewExplosive));
            return;
        }
    }
}

UFUNCTION() void UInventoryListUI::OrderAlphabetically()
{
    //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("사전 순 정렬")));
    OrderInventory("Alphabet");
}

UFUNCTION() void UInventoryListUI::OrderRecently()
{
    //GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, FString::Printf(TEXT("최근 순 정렬")));
    OrderInventory("Recent");
}