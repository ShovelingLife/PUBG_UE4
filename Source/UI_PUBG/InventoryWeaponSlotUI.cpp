#include "InventoryWeaponSlotUI.h"
#include "CursorSlotUI.h"
#include "InventoryListUI.h"
#include "InventoryManager.h"
#include "InventoryUI.h"
#include "ItemSlotUI.h"
#include "WeaponSocketUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Farmable_items/CoreAttachment.h"
#include "Farmable_items/CoreBarrel.h"
#include "Farmable_items/CoreForend.h"
#include "Farmable_items/CoreGrip.h"
#include "Farmable_items/CoreSight.h"
#include "Farmable_items/CoreStock.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Sizebox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

void UInventoryWeaponSlotUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitWeaponSlot();

    auto p_gameInst = UCustomGameInstance::GetInst();
    pGameInstanceSubSystemUI = UGameInstance::GetSubsystem<UGameInstanceSubsystemUI>(p_gameInst);    
    //pGameInstanceSubSystemUI->DeleVerifyAttachmentSlot.BindUFunction(this, "VerifyAttachmentSlot");
    p_gameInst->DeleSetExplosiveUI.BindUFunction(this, "UpdateThrowable");
    mArrCanvasPanel.Add(FirstGunCanvasPanel);
    mArrCanvasPanel.Add(SecondGunCanvasPanel);
    mArrCanvasPanel.Add(PistolCanvasPanel);
    mArrCanvasPanel.Add(MeleeCanvasPanel);
    mArrCanvasPanel.Add(GrenadeCanvasPanel);
}

void UInventoryWeaponSlotUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

    if (mpWeaponManager)
    {
        mArrWeapon[0] = mpWeaponManager->pFirstGun;
        mArrWeapon[1] = mpWeaponManager->pSecondGun;
        mArrWeapon[2] = mpWeaponManager->pPistol;

        UpdateWeaponSlotVisibility();
        UpdateInventoryWeaponUI();
    }
    else
        mpWeaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    CheckForHoveredWeaponSlot();
}

FReply UInventoryWeaponSlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if (!pGameInstanceSubSystemUI)
        return FReply::Unhandled();

    // 왼쪽 클릭 / 마우스 커서 변경
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        if (mpWeaponManager)
            mItemData = FsSlotItemData::GetDataFrom(mpWeaponManager->GetWeaponByIndex(mSelectedWeaponType));

        HighlightImg->SetColorAndOpacity({ 1.f,1.f,1.f,0.35f });
        mbClicked = true;
        mDraggedWeaponType = mSelectedWeaponType;
    }
    // 오른쪽 클릭 / 맵에다가 무기를 버림
    else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        if (mpWeaponManager)
            mpWeaponManager->Drop(mSelectedWeaponType);

        ResetHighlightImg();
    }
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UInventoryWeaponSlotUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    mItemData.Reset();
    ResetHighlightImg();
    return FReply::Handled();
}

void UInventoryWeaponSlotUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    if (!mbClicked)
        ResetHighlightImg();
}

FReply UInventoryWeaponSlotUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    mCurrentPointerEvent = InMouseEvent;
    return FReply::Handled();
}


void UInventoryWeaponSlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    ABaseInteraction* p_weapon = nullptr;    

    // 슬롯 설정
    UItemSlotUI* p_slot = CreateWidget<UItemSlotUI>(GetOwningPlayer(), BP_ItemSlotUI);
    FVector2D mousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (mpWeaponManager)
        p_weapon = mpWeaponManager->GetWeaponByIndex(mSelectedWeaponType);
        
    if (!p_slot   ||
        !p_weapon ||
        mItemData.IsEmpty())
        return;

    p_slot->pDraggedItem = p_weapon;
    p_slot->ItemData = mItemData;
    p_slot->DeleDeleteFromList.BindUFunction(this, "SetSlotNull");    

    // 드래그 구현
    if (auto p_customOperation = NewObject<UCustomDragDropOperation>())
    {
        // 커서 슬롯 생성
        p_customOperation->Init(p_slot, mousePos);

        if (auto p_subGameInstUI = UGameInstanceSubsystemUI::GetInst())
            p_customOperation->DefaultDragVisual = p_subGameInstUI->GetSlotCursorUI(mItemData, mousePos);

        if ((int)mSelectedWeaponType < 4)
            p_customOperation->bGun = true;

        OutOperation = p_customOperation;
    }
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryWeaponSlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    auto p_customOperation = Cast<UCustomDragDropOperation>(InOperation);
    auto p_slot = p_customOperation->GetSlot();
    
    if (!pGameInstanceSubSystemUI ||
        !mpWeaponManager ||
        !p_slot)    
        return false;   

    // 현재 선택하고 있는 인덱스 확인
    auto p_draggedWeapon = p_slot->pDraggedItem;

    // 맞지 않는 슬롯에 위치시킬 시
    if (mpWeaponManager->IsWrongType(p_draggedWeapon, mSelectedWeaponType, p_customOperation->bWeaponSlot))
        return false;

    // 무기 선택
    ABaseInteraction* p_selectedWeapon = mpWeaponManager->GetWeaponByIndex(mSelectedWeaponType);
    
    if (mpWeaponManager->Swap(p_draggedWeapon, p_selectedWeapon, mSelectedWeaponType) == -1)
        return false;

    // 무기 배치 및 UI 설정이 완료 되었다면 초기화
    if (!p_selectedWeapon)
    {
        if (p_slot->ItemData.Count > 1)
            p_slot->DeleChangeItemCount.ExecuteIfBound(p_draggedWeapon, false);

        else
            p_slot->DeleDeleteFromList.ExecuteIfBound();
    }
    else
    {
        if (!p_customOperation->bInventoryList)
            p_slot->DeleSwapWeaponSlot.ExecuteIfBound(p_slot);

        else
            p_slot->DeleSwapInventoryExplosive.ExecuteIfBound(Cast<ACoreThrowableWeapon>(p_selectedWeapon), Cast<ACoreThrowableWeapon>(p_draggedWeapon));
    }
    p_customOperation->Classify("Weapon");
    return true;
}

void UInventoryWeaponSlotUI::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    CheckForHoveredWeaponSlot();
}

void UInventoryWeaponSlotUI::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragLeave(InDragDropEvent, InOperation);
    ResetHighlightImg();
}

void UInventoryWeaponSlotUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    ResetHighlightImg();
}

bool UInventoryWeaponSlotUI::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{ 
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
    FVector2D dummyVec = FVector2D::ZeroVector, widgetPos = FVector2D::ZeroVector;
    //USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), mArrCanvasPanel[((int)mSelectedWeaponIndex) - 1]->GetCachedGeometry().GetAbsolutePosition(), dummyVec, widgetPos);
    
    if      (mousePos.Y <= 306.f)
             mSelectedWeaponType = FIRST;

    else if (mousePos.Y <= 513.f)
             mSelectedWeaponType = SECOND;

    else if  (mousePos.Y <= 719.f)
             mSelectedWeaponType = PISTOL;

    else
    {
        if      (mousePos.X <= 1550.f)
                 mSelectedWeaponType = MELEE;

        else if (mousePos.X >= 1584.f)
                 mSelectedWeaponType = THROWABLE;
    }
    //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::FromInt((int)mSelectedWeaponType));
    return true;
}

void UInventoryWeaponSlotUI::InitWeaponSlot()
{
    using enum EAttachmentType;

    // 첫번째 총기 부착품
    FirstGunScopeSlot->AttachmentType    = FSCOPE;
    FirstGunStockSlot->AttachmentType    = FSTOCK;
    FirstGunGripSlot->AttachmentType     = FGRIP;
    FirstGunMagazineSlot->AttachmentType = FMAGAZINE;
    FirstGunMuzzleSlot->AttachmentType   = FMUZZLE;

    // 두번째 총기 부착품
    FirstGunScopeSlot->AttachmentType    = SSCOPE;
    FirstGunStockSlot->AttachmentType    = SSTOCK;
    FirstGunGripSlot->AttachmentType     = SGRIP;
    FirstGunMagazineSlot->AttachmentType = SMAGAZINE;
    FirstGunMuzzleSlot->AttachmentType   = SMUZZLE;

    // 세번째 총기 부착품
    PistolScopeSlot->AttachmentType    = FSCOPE;
    PistolMagazineSlot->AttachmentType = FMAGAZINE;
    PistolMuzzleSlot->AttachmentType   = FMUZZLE;
}

void UInventoryWeaponSlotUI::UpdateWeaponSlotVisibility()
{
    FirstGunCanvasPanel->SetVisibility(          (mpWeaponManager->pFirstGun)  ? VISIBLE : HIDDEN);
    SecondGunCanvasPanel->SetVisibility(         (mpWeaponManager->pSecondGun) ? VISIBLE : HIDDEN);
    PistolCanvasPanel->SetVisibility(            (mpWeaponManager->pPistol)    ? VISIBLE : HIDDEN);
    MeleeCanvasPanel->SetVisibility ( (mpWeaponManager->pMelee)     ? VISIBLE : HIDDEN);
    GrenadeCanvasPanel->SetVisibility((mpWeaponManager->pThrowable) ? VISIBLE : HIDDEN);
}

TArray<UTexture*> UInventoryWeaponSlotUI::GetAttachmentTexArr(ACoreWeapon* pWeapon) const
{
    TArray<UTexture*> arrAttachmentTex;
    
    if (!pWeapon)
        return arrAttachmentTex;

    ACoreBarrel* p_barrel = pWeapon->CurrentBarrel;
    ACoreGrip*   p_grip   = pWeapon->CurrentGrip;
    ACoreForend* p_forend = pWeapon->CurrentForend;
    ACoreSight*  p_scope  = pWeapon->CurrentSight;
    ACoreStock*  p_stock  = pWeapon->CurrentStock;

    // 텍스처 설정
    arrAttachmentTex.Add((p_barrel) ? p_barrel->CurrentItemTex : nullptr);
    arrAttachmentTex.Add((p_grip)   ? p_grip->CurrentItemTex   : nullptr);
    arrAttachmentTex.Add((p_forend) ? p_forend->CurrentItemTex : nullptr);
    arrAttachmentTex.Add((p_stock)  ? p_stock->CurrentItemTex  : nullptr);
    arrAttachmentTex.Add((p_scope)  ? p_scope->CurrentItemTex  : nullptr);
    return arrAttachmentTex;
}

void UInventoryWeaponSlotUI::UpdateInventoryWeaponUI()
{
    // 첫번째 무기
    if (auto p_firstGun = mpWeaponManager->pFirstGun)
    {
        auto weaponData = p_firstGun->WeaponData;
        FirstGunSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_firstGun->WeaponType, "Gun"));
        FirstGunNameTxt->SetText(FText::FromString(weaponData.Type));
        FirstGunBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
    }
    // 두번째 무기
    if (auto p_secondGun = mpWeaponManager->pSecondGun)
    {
        auto weaponData = p_secondGun->WeaponData;
        SecondGunSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_secondGun->WeaponType, "Gun"));
        SecondGunNameTxt->SetText(FText::FromString(weaponData.Type));
        SecondGunBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
    }
    // 세번째 무기
    if (auto p_pistol = mpWeaponManager->pPistol)
    {
        auto weaponData = p_pistol->WeaponData;
        PistolSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_pistol->WeaponType, "Gun"));
        PistolNameTxt->SetText(FText::FromString(weaponData.Type));
        PistolBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
    }
    // 네번째 무기
    if (auto p_melee = mpWeaponManager->pMelee)
    {
        MeleeSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_melee->WeaponType, "Explosive"));
        MeleeNameTxt->SetText(FText::FromString(p_melee->WeaponData.Type));
    }
}

void UInventoryWeaponSlotUI::CheckForHoveredWeaponSlot()
{
    // 총 무기 다섯칸 중 어느거 선택했는지 확인 후 인덱스 구함
    for (int i = 0; i < mArrCanvasPanel.Num(); i++)
    {
        auto weaponCanvas = mArrCanvasPanel[i];

        if (weaponCanvas->IsHovered())
        {
            ResetHighlightImg();
            mSelectedWeaponType = static_cast<EWeaponType>(i + 1);
            UpdateHighlightImgPos();
            break;
        }
    }
}
 
void UInventoryWeaponSlotUI::UpdateHighlightImgPos()
{
    // 무기가 선택됐을 시 이미지 설정
    auto idx = (int)mSelectedWeaponType - 1;
    //GEngine->AddOnScreenDebugMessage(8, 1.f, FColor::Red, FString::FromInt(idx));

    if (mSelectedWeaponType == NONE ||
        idx < 0)
    {
        ResetHighlightImg();
        return;
    }
    if (UCanvasPanelSlot* p_canvasPanelSlot = mArrCanvasPanel[idx]->AddChildToCanvas(HighlightImg))
    {
        float sizeX = (idx < 3) ? 485.f : 225.f;
        p_canvasPanelSlot->SetSize({ sizeX, 183.5f });
        HighlightImg->SetVisibility(VISIBLE);
        HighlightImg->SetColorAndOpacity({ 1.f,1.f,1.f,0.1f });
    }
}

void UInventoryWeaponSlotUI::ResetHighlightImg()
{
    int idx = (int)mSelectedWeaponType - 1;

    if (mSelectedWeaponType == NONE ||
        idx < 0)
        return;

    // 선택 이미지 부모 새로 지정 및 무기 슬롯 이미지 설정
    mArrCanvasPanel[idx]->RemoveChild(HighlightImg);
    MainCanvasPanel->AddChildToCanvas(HighlightImg);
    HighlightImg->SetVisibility(HIDDEN);
    mSelectedWeaponType = NONE;
    mbClicked = false;
}

int UInventoryWeaponSlotUI::GetAttachmentSlotIndex(FString AttachmentType) const
{
    TMap<FString, int> mapAttachmentIdx
    {
        { "Scope",  0 }, { "Sight",  0 }, { "IRS",    0 },
        { "Stock",  1 }, { "StockA", 1 }, { "StockB", 1 },
        { "Grip",   2 },
        { "Forend", 3 },
        { "Barrel", 4 }
    };
    if (!mapAttachmentIdx.Contains(AttachmentType))
        return -1;

    return mapAttachmentIdx[AttachmentType];
}

void UInventoryWeaponSlotUI::SetSlotNull()
{
    if (mpWeaponManager)
    {
        mpWeaponManager->SetNull(mDraggedWeaponType);
        ResetHighlightImg();
        mDraggedWeaponType = NONE;
        mItemData.Reset();
    }
}

UFUNCTION() void UInventoryWeaponSlotUI::UpdateThrowable(ACoreThrowableWeapon* pThrowable)
{
    TArray<UWidget*> arrThrowableWidget
    {
        GrenadeSlotImg,
        GrenadeNameTxt,
        GrenadeNumberBackground
    };
    // 수류탄 UI 설정
    if (pThrowable)
    {
        GrenadeSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)pThrowable->WeaponType, "Explosive"));
        GrenadeNameTxt->SetText(FText::FromString(pThrowable->WeaponData.Type));
    }
    for (int i = 0; i < arrThrowableWidget.Num(); i++)
        arrThrowableWidget[i]->SetVisibility(pThrowable ? VISIBLE : HIDDEN);
}