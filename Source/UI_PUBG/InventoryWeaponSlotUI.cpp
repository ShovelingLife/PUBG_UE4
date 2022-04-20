﻿#include "InventoryWeaponSlotUI.h"
#include "InventoryListUI.h"
#include "InventoryManager.h"
#include "InventoryUI.h"
#include "ItemSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Sizebox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWeaponSlotUI::NativeConstruct()
{
    Super::NativeConstruct();
    HideAllSlotUI_background();

    pGameInstanceSubSystemUI = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();
}

void UInventoryWeaponSlotUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    CheckForHoveredWeaponSlot();
    UpdateVisibility();
    UpdateInventoryWeaponUI();
    UpdateHighlightImgPos(); 
}

void UInventoryWeaponSlotUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    if (!mbClicked)
        ResetHighlightImg();
}

FReply UInventoryWeaponSlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    
    if (!pGameInstanceSubSystemUI)
        return FReply::Unhandled();

    // 왼쪽 클릭 / 마우스 커서 변경
    if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
            mItemData = FsSlotItemData::GetDataFrom(p_weaponManager->GetWeaponByIndex(mSelectedWeaponIndex));

        HighlightImg->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.35f });
        mbClicked           = true;
        mDraggedWeaponIndex = mSelectedWeaponIndex;
    }
    // 오른쪽 클릭 / 맵에다가 무기를 버림
    else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
            p_weaponManager->Drop(mSelectedWeaponIndex);

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
void UInventoryWeaponSlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    ABaseInteraction* p_weapon = nullptr;

    if (pGameInstanceSubSystemUI)
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
            p_weapon= p_weaponManager->GetWeaponByIndex(mSelectedWeaponIndex);
    }
    auto      p_slot   = CreateWidget<UItemSlotUI>(GetWorld(), BP_itemSlotUI);
    FVector2D mousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (!p_slot   ||
        !p_weapon ||
        mItemData.IsEmpty())
        return;

    // 슬롯 설정
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
        subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(nullptr, ESlateVisibility::Hidden);

    p_slot->pDraggedItem = p_weapon;
    p_slot->ItemData     = mItemData;
    p_slot->Priority     = 1;
    p_slot->DeleSetSlotNull.BindUFunction(this, "SetSlotNull");
    p_slot->SetAsCursor(mousePos);

    // 드래그 구현
    auto p_customOperation = NewObject<UCustomDragDropOperation>();

    if ((int)mSelectedWeaponIndex < 4)
        p_customOperation->bGun = true;

    p_customOperation->pSlotUI           = p_slot;
    p_customOperation->DefaultDragVisual = p_slot;
    p_customOperation->Pivot             = EDragPivot::MouseDown;
    p_customOperation->ItemData          = mItemData;
    OutOperation = p_customOperation;
}

bool UInventoryWeaponSlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    auto p_customOperation = Cast<UCustomDragDropOperation>(InOperation);
    auto p_slot = p_customOperation->pSlotUI;
    AWeaponManager* p_weaponManager = nullptr;

    if (pGameInstanceSubSystemUI)
        p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    if (!p_weaponManager ||
        !p_slot)
        return false;

    p_customOperation->bFromList = false;

    // 현재 선택하고 있는 인덱스 확인
    auto p_draggedWeapon  = p_slot->pDraggedItem;

    if (!p_draggedWeapon)
        return false;

    // 총기가 수류탄 또는 근접무기 슬롯에 위치시킬 시
    if ((int)mSelectedWeaponIndex < 4 &&
        p_weaponManager->IsDuplicated(p_draggedWeapon, ECurrentWeaponType::NONE))
        return false;

    // 1 2번 슬롯 같은 경우 무기가 중복 여부 같은 슬롯 내에는 발생하지 않음
    if (p_customOperation->bFromList)
    {
        if (p_weaponManager->IsDuplicated(p_draggedWeapon, mSelectedWeaponIndex))
            return false;
    }
    // 무기 선택
    ABaseInteraction* p_selectedWeapon = p_weaponManager->GetWeaponByIndex((ECurrentWeaponType)mSelectedWeaponIndex);
    mItemData = (!p_selectedWeapon) ? FsSlotItemData::GetDataFrom(p_draggedWeapon) : FsSlotItemData::GetDataFrom(p_selectedWeapon);

    p_slot->ItemData = mItemData;
    p_slot->pDraggedItem = p_selectedWeapon;

    int exitCode = p_weaponManager->Swap(p_draggedWeapon, p_selectedWeapon, mSelectedWeaponIndex);

    if (exitCode == -1)
        return false;

    else if (exitCode == 1) // 근접 또는 투척류
    {
        if (auto p_customGameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
        {
            p_customGameInst->DeleSetItemOntoInventory.ExecuteIfBound(p_selectedWeapon, true);
            return true;
        }
    }
    // 무기 배치 및 UI 설정이 완료 되었다면 초기화
    if (!p_selectedWeapon)
        p_slot->DeleSetSlotNull.ExecuteIfBound();

    else
        p_slot->DeleSwapWeaponSlot.ExecuteIfBound(p_slot);

    return true;
}

void UInventoryWeaponSlotUI::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
    ResetHighlightImg();
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
    ResetHighlightImg();

     TArray< UCanvasPanel* >p_arrCanvasPanel
    {
        FirstGunCanvasPanel,
        SecondGunCanvasPanel,
        PistolCanvasPanel,
        MeleeCanvasPanel,
        GrenadeCanvasPanel
    };
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
    FVector2D dummyVec = FVector2D::ZeroVector, widgetPos = FVector2D::ZeroVector;

    // 어떤 무기를 선택했는지 확인
    for (int i = 0; i < p_arrCanvasPanel.Num(); i++)
    {
        USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), p_arrCanvasPanel[i]->GetCachedGeometry().GetAbsolutePosition(), dummyVec, widgetPos);

        if (i < 3) // 총기류
        {
            if (mousePos.Y >= widgetPos.Y)
                mSelectedWeaponIndex = (ECurrentWeaponType)(i + 1);
        }
        else // 투척류 또는 근접일 시 두 축으로 비교
        {
            if (mousePos.Y >= widgetPos.Y &&
                mousePos.X >= widgetPos.X)
                mSelectedWeaponIndex = (ECurrentWeaponType)(i + 1);
        }
    }
    return true;
}

void UInventoryWeaponSlotUI::UpdateVisibility()
{
    if (!pGameInstanceSubSystemUI)
        return;

    auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    if (!p_weaponManager)
        return;

    // 첫번째 무기
    TArray<UWidget*> arrFirstGunWidget
    {
        FirstGunSlotImg,
        FirstGunNameTxt,
        FirstGunBulletTypeTxt,
        FirstGunCurrentMagazineTxt,
        FirstGunMaxMagazineTxt,
        FirstGunNumberBackground
    };
    // 두번째 무기
    TArray<UWidget*> arrSecondGunWidget
    {
        SecondGunSlotImg,
        SecondGunNameTxt,
        SecondGunBulletTypeTxt,
        SecondGunCurrentMagazineTxt,
        SecondGunMaxMagazineTxt,
        SecondGunNumberBackground
    };
    // 세번째 무기
    TArray<UWidget*> arrPistolWidget
    {
        PistolSlotImg,
        PistolNameTxt,
        PistolBulletTypeTxt,
        PistolCurrentMagazineTxt,
        PistolMaxMagazineTxt,
        PistolNumberBackground
    };
    // 네번째 무기
    TArray<UWidget*> arrMeleeWidget
    {
        MeleeSlotImg,
        MeleeNameTxt,
        MeleeNumberBackground

    };
    // 다섯번째 무기
    TArray<UWidget*> arrThrowableWidget
    {
        GrenadeSlotImg,
        GrenadeNameTxt,
        GrenadeNumberBackground
    };

    for (auto item : arrFirstGunWidget)
    {
        if (item)
            item->SetVisibility((p_weaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    for (auto item : arrSecondGunWidget)
    {
        if (item)
            item->SetVisibility((p_weaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    for (auto item : arrPistolWidget)
    {
        if (item)
            item->SetVisibility((p_weaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    for (auto item : arrMeleeWidget)
    {
        if (item)
            item->SetVisibility((p_weaponManager->pMelee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    for (auto item : arrThrowableWidget)
    {
        if (item)
            item->SetVisibility((p_weaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UInventoryWeaponSlotUI::UpdateInventoryWeaponUI()
{
    if (pGameInstanceSubSystemUI)
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
        {
            // 첫번째 무기
            if (auto p_firstGun = p_weaponManager->pFirstGun)
            {
                auto weaponData = p_firstGun->WeaponData;
                FirstGunSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_firstGun->WeaponType, "Gun"));
                FirstGunNameTxt->SetText(FText::FromString(weaponData.Type));
                FirstGunBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
            }
            // 두번째 무기
            if (auto p_secondGun = p_weaponManager->pSecondGun)
            {
                auto weaponData = p_secondGun->WeaponData;
                SecondGunSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_secondGun->WeaponType, "Gun"));
                SecondGunNameTxt->SetText(FText::FromString(weaponData.Type));
                SecondGunBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
            }
            // 세번째 무기
            if (auto p_pistol = p_weaponManager->pPistol)
            {
                auto weaponData = p_pistol->WeaponData;
                PistolSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_pistol->WeaponType, "Gun"));
                PistolNameTxt->SetText(FText::FromString(weaponData.Type));
                PistolBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
            }
            // 네번째 무기
            if (auto p_melee = p_weaponManager->pMelee)
            {
                MeleeSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_melee->CurrentWeaponType));
                MeleeNameTxt->SetText(FText::FromString(p_melee->WeaponData.Type));
            }
            // 다섯번째 무기
            if (auto p_throwable = p_weaponManager->pThrowable)
            {
                GrenadeSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_throwable->CurrentWeaponType));
                GrenadeNameTxt->SetText(FText::FromString(p_throwable->WeaponData.Type));
            }
        }
    }
}

void UInventoryWeaponSlotUI::CheckForHoveredWeaponSlot()
{
    if (mbClicked)
        return;

    // 선택된 무기 인덱스 구함
    TArray<Chaos::Pair<UImage*, ECurrentWeaponType>> arrWeaponImg
    {
        Chaos::MakePair<UImage*, ECurrentWeaponType>(FirstGunSlotImg,  ECurrentWeaponType::FIRST),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(SecondGunSlotImg, ECurrentWeaponType::SECOND),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(PistolSlotImg,    ECurrentWeaponType::PISTOL),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(MeleeSlotImg,     ECurrentWeaponType::MELEE),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(GrenadeSlotImg,   ECurrentWeaponType::THROWABLE)
    };
    for (auto item : arrWeaponImg)
    {
        auto weaponImg = item.First;

        if (weaponImg->GetVisibility() == ESlateVisibility::Visible &&
            weaponImg->IsHovered())
            mSelectedWeaponIndex = item.Second;
    }
}

void UInventoryWeaponSlotUI::UpdateHighlightImgPos()
{
    if (mbClicked)
        return;

    float sizeX = 0.f;    
    UCanvasPanelSlot* p_canvasPanelSlot = nullptr;

    // 무기가 선택됐을 시 이미지 설정
    switch(mSelectedWeaponIndex)
    {
    case ECurrentWeaponType::FIRST:     p_canvasPanelSlot = FirstGunCanvasPanel->AddChildToCanvas(HighlightImg);  sizeX = 545.f; break;
    case ECurrentWeaponType::SECOND:    p_canvasPanelSlot = SecondGunCanvasPanel->AddChildToCanvas(HighlightImg); sizeX = 545.f; break;
    case ECurrentWeaponType::PISTOL:    p_canvasPanelSlot = PistolCanvasPanel->AddChildToCanvas(HighlightImg);    sizeX = 545.f; break;
    case ECurrentWeaponType::MELEE:     p_canvasPanelSlot = MeleeCanvasPanel->AddChildToCanvas(HighlightImg);     sizeX = 225.f; break;
    case ECurrentWeaponType::THROWABLE: p_canvasPanelSlot = GrenadeCanvasPanel->AddChildToCanvas(HighlightImg);   sizeX = 225.f; break;
    }
    if (mSelectedWeaponIndex != ECurrentWeaponType::NONE &&
        p_canvasPanelSlot)
    {
        p_canvasPanelSlot->SetSize(FVector2D(sizeX, 191.f));
        HighlightImg->SetVisibility(ESlateVisibility::Visible);
        HighlightImg->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.1f });
    }
}

void UInventoryWeaponSlotUI::ResetHighlightImg()
{
    // 선택 이미지 부모 새로 지정 및 무기 슬롯 이미지 설정
    switch (mSelectedWeaponIndex)
    {
    case ECurrentWeaponType::FIRST:     FirstGunCanvasPanel->RemoveChild(HighlightImg);  break;
    case ECurrentWeaponType::SECOND:    SecondGunCanvasPanel->RemoveChild(HighlightImg); break;
    case ECurrentWeaponType::PISTOL:    PistolCanvasPanel->RemoveChild(HighlightImg);    break;
    case ECurrentWeaponType::THROWABLE: MeleeCanvasPanel->RemoveChild(HighlightImg);     break;
    case ECurrentWeaponType::NONE:      GrenadeCanvasPanel->RemoveChild(HighlightImg);   break;
    }
    MainCanvasPanel->AddChildToCanvas(HighlightImg);
    HighlightImg->SetVisibility(ESlateVisibility::Hidden);
    mSelectedWeaponIndex = ECurrentWeaponType::NONE;
    mbClicked = false;
}

void UInventoryWeaponSlotUI::HideAllSlotUI_background()
{
    FirstGunMuzzleSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    FirstGunGripSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    FirstGunMagazineSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    FirstGunStockSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    FirstGunScopeSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    
    SecondGunMuzzleSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    SecondGunGripSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    SecondGunMagazineSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    SecondGunStockSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    SecondGunScopeSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    
    PistolMuzzleSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    PistolMagazineSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
    PistolScopeSlotUI->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWeaponSlotUI::SetSlotNull()
{
    if (!pGameInstanceSubSystemUI)
        return;

    if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
    {
        p_weaponManager->SetNull((ECurrentWeaponType)mDraggedWeaponIndex);
        ResetHighlightImg();
        mDraggedWeaponIndex = ECurrentWeaponType::NONE;
        mItemData.Reset();
    }
}