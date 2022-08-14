#include "InventoryWeaponSlotUI.h"
#include "InventoryListUI.h"
#include "InventoryManager.h"
#include "InventoryUI.h"
#include "ItemSlotUI.h"
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
#include "Kismet/KismetStringLibrary.h"

void UInventoryWeaponSlotUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitSettings();
    UpdateThrowable(nullptr);

    if (auto pWorld = GetWorld())
    {
        pGameInstanceSubSystemUI = UGameInstance::GetSubsystem<UGameInstanceSubsystemUI>(pWorld->GetGameInstance());
        pGameInstanceSubSystemUI->DeleVerifyAttachmentSlot.BindUFunction(this, "VerifyAttachmentSlot");
    }
}

void UInventoryWeaponSlotUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

    if (pGameInstanceSubSystemUI &&
        !mpWeaponManager)
        mpWeaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    if (mpWeaponManager)
    {
        auto deleSetExplosive = mpWeaponManager->DeleSetExplosiveUI;

        if (!deleSetExplosive.IsBound())
            deleSetExplosive.BindUFunction(this, "UpdateThrowable");

        mArrWeapon[0] = mpWeaponManager->pFirstGun;
        mArrWeapon[1] = mpWeaponManager->pSecondGun;
        mArrWeapon[2] = mpWeaponManager->pPistol;

        SetWeaponSlotVisibility();
        UpdateInventoryWeaponUI();
        UpdateAttachmentSlot();
    }
    if (!mbClicked)
    {
        UpdateHighlightImgPos();
        CheckForHoveredWeaponSlot();
        //CheckForHoveredAttachmentSlot();
    }
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
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        if (mpWeaponManager)
            mItemData = FsSlotItemData::GetDataFrom(mpWeaponManager->GetWeaponByIndex(mSelectedWeaponIndex));

        HighlightImg->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.35f });
        mbClicked = true;
        mDraggedWeaponIndex = mSelectedWeaponIndex;
    }
    // 오른쪽 클릭 / 맵에다가 무기를 버림
    else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        if (mpWeaponManager)
            mpWeaponManager->Drop(mSelectedWeaponIndex);

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

    if (mpWeaponManager)
        p_weapon = mpWeaponManager->GetWeaponByIndex(mSelectedWeaponIndex);

    auto      p_slot   = CreateWidget<UItemSlotUI>(GetWorld(), BP_itemSlotUI);
    FVector2D mousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (!p_slot ||
        !p_weapon ||
        mItemData.IsEmpty())
        return;

    // 슬롯 설정
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
        subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(nullptr, HIDDEN);

    p_slot->pDraggedItem = p_weapon;
    p_slot->ItemData = mItemData;
    p_slot->Priority = 1;
    p_slot->DeleDeleteFromList.BindUFunction(this, "SetSlotNull");
    p_slot->SetAsCursor(mousePos);

    // 드래그 구현
    if (auto p_customOperation = NewObject<UCustomDragDropOperation>())
    {
        if ((int)mSelectedWeaponIndex < 4)
            p_customOperation->bGun = true;

        p_customOperation->pSlotUI = p_slot;
        p_customOperation->DefaultDragVisual = p_slot;
        p_customOperation->Pivot = EDragPivot::MouseDown;
        p_customOperation->bWeaponSlot = true;
        OutOperation = p_customOperation;
    }
}

bool UInventoryWeaponSlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    auto p_customOperation = Cast<UCustomDragDropOperation>(InOperation);

    if (!p_customOperation ||
        !pGameInstanceSubSystemUI)
        return false;

    auto p_slot = p_customOperation->pSlotUI;

    if (!mpWeaponManager ||
        !p_slot)
        return false;

    // 현재 선택하고 있는 인덱스 확인
    auto p_draggedWeapon = p_slot->pDraggedItem;

    // 맞지 않는 슬롯에 위치시킬 시
    if (mpWeaponManager->IsWrongType(p_draggedWeapon, mSelectedWeaponIndex, p_customOperation->bWeaponSlot))
        return false;

    // 무기 선택
    ABaseInteraction* p_selectedWeapon = mpWeaponManager->GetWeaponByIndex((EWeaponType)mSelectedWeaponIndex);
    
    if (mpWeaponManager->Swap(p_draggedWeapon, p_selectedWeapon, mSelectedWeaponIndex) == -1)
        return false;

    // 무기 배치 및 UI 설정이 완료 되었다면 초기화
    if (!p_selectedWeapon)
    {
        auto itemData = p_slot->ItemData;

        if (itemData.Count > 1)
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
    p_customOperation->bInventoryList = false;
    p_customOperation->bWeaponSlot    = false;
    p_customOperation->pSlotUI        = p_slot;
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
                mSelectedWeaponIndex = (EWeaponType)(i + 1);
        }
        else // 투척류 또는 근접일 시 두 축으로 비교
        {
            if (mousePos.Y >= widgetPos.Y &&
                mousePos.X >= widgetPos.X)
                mSelectedWeaponIndex = (EWeaponType)(i + 1);
        }
    }
    return true;
}

void UInventoryWeaponSlotUI::InitSettings()
{
    TArray< UCanvasPanel*> arrCanvasPanel
    {
        FirstGunCanvasPanel,
        SecondGunCanvasPanel,
        PistolCanvasPanel
    };
    for (int i = 0; i < arrCanvasPanel.Num(); i++)
    {
    	for (auto UI_item : arrCanvasPanel[i]->GetAllChildren())
    	{
            FString UI_itemName = (UI_item) ? UI_item->GetName() : "";
            
            // 뒷배경 Border > r
            if (UI_itemName.EndsWith("r"))
            {
                auto p_border = Cast<UBorder>(UI_item);

                switch (i)
                {
                case 0: mArrFirstGunAttachmentBorder.Add(p_border);  break;
                case 1: mArrSecondGunAttachmentBorder.Add(p_border); break;
                case 2: mArrPistolAttachmentBorder.Add(p_border);    break;
                }
                // 슬롯 UI는 뒷배경의 자식 UI임
                if (p_border)
                {
                    if (auto p_slotUI = Cast<UItemSlotUI>(p_border->GetChildAt(0)))
                    {
                        switch (i)
                        {
                        case 0: mArrFirstGunAttachmentUI.Add(p_slotUI);  break;
                        case 1: mArrSecondGunAttachmentUI.Add(p_slotUI); break;
                        case 2: mArrPistolAttachmentUI.Add(p_slotUI);    break;
                        }
                    }
                }
            }
    	}
    }
    for (int i = 0; i < mkTotalGunAttachmentUI; i++)
    {
        if (auto p_firstGunAttachmentUI = mArrFirstGunAttachmentUI[i])
            p_firstGunAttachmentUI->SetForAttachmentUI();

        if (auto p_secondGunAttachmentUI = mArrSecondGunAttachmentUI[i])
            p_secondGunAttachmentUI->SetForAttachmentUI();
    }
    for (int i = 0; i < mkTotalPistolAttachmentUI; i++)
    {
        if (auto p_pistolAttachmentUI = mArrPistolAttachmentUI[i])
            p_pistolAttachmentUI->SetForAttachmentUI();
    }
}

void UInventoryWeaponSlotUI::SetWeaponSlotVisibility()
{
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
    // 총기류 설정
    for (int i = 0; i < 6; i++)
    {
        arrFirstGunWidget[i]->SetVisibility((mpWeaponManager->pFirstGun) ? VISIBLE : HIDDEN);
        arrSecondGunWidget[i]->SetVisibility((mpWeaponManager->pSecondGun) ? VISIBLE : HIDDEN);
        arrPistolWidget[i]->SetVisibility((mpWeaponManager->pPistol) ? VISIBLE : HIDDEN);
    }
    // 기타 무기 설정
    for (int i = 0; i < 3; i++)
         arrMeleeWidget[i]->SetVisibility((mpWeaponManager->pMelee) ? VISIBLE : HIDDEN);
}

void UInventoryWeaponSlotUI::UpdateAttachmentSlot()
{
    // 총기 부착물 UI 갱신
    auto firstGunAttachmentTexArr  = GetAttachmentTexArr(mpWeaponManager->pFirstGun);
    auto secondGunAttachmentTexArr = GetAttachmentTexArr(mpWeaponManager->pSecondGun);
    auto pistolAttachmentTexArr    = GetAttachmentTexArr(mpWeaponManager->pPistol);

    for (int i = 0; i < mkTotalGunAttachmentUI; i++)
    {
        // 첫번째 무기
        if (!firstGunAttachmentTexArr.IsEmpty())
        {
            // 뒷배경
            if (auto p_firstGunAttachmentBorder = mArrFirstGunAttachmentBorder[i])
                p_firstGunAttachmentBorder->SetVisibility((mpWeaponManager->pFirstGun) ? VISIBLE : HIDDEN);

            // 아이템 UI
            if (auto p_firstGunAttachmentUI = mArrFirstGunAttachmentUI[i])
                p_firstGunAttachmentUI->ItemImg->SetBrushFromTexture(Cast<UTexture2D>(firstGunAttachmentTexArr[i]));
        }
        // 두번째 무기
        if (!secondGunAttachmentTexArr.IsEmpty())
        {
            // 뒷배경
            if (auto p_secondGunAttachmentBorder = mArrSecondGunAttachmentBorder[i])
                p_secondGunAttachmentBorder->SetVisibility((mpWeaponManager->pSecondGun) ? VISIBLE : HIDDEN);

            // 아이템 UI
            if (auto p_secondGunAttachmentUI = mArrSecondGunAttachmentUI[i])
                p_secondGunAttachmentUI->ItemImg->SetBrushFromTexture(Cast<UTexture2D>(secondGunAttachmentTexArr[i]));
        }
        // 세번쨰 무기
        if (i < mkTotalPistolAttachmentUI)
        {
            if (!pistolAttachmentTexArr.IsEmpty())
            {
                // 뒷배경
                if (auto p_pistolAttachmentBorder = mArrPistolAttachmentBorder[i])
                    p_pistolAttachmentBorder->SetVisibility((mpWeaponManager->pPistol) ? VISIBLE : HIDDEN);

                // 아이템 UI
                if (auto p_pistolAttachmentUI = mArrFirstGunAttachmentUI[i])
                    p_pistolAttachmentUI->ItemImg->SetBrushFromTexture(Cast<UTexture2D>(pistolAttachmentTexArr[i]));
            }
        }
    }
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
        MeleeSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_melee->WeaponType));
        MeleeNameTxt->SetText(FText::FromString(p_melee->WeaponData.Type));
    }
}

void UInventoryWeaponSlotUI::CheckForHoveredWeaponSlot()
{
    // 선택된 무기 인덱스 구함
    TArray<TPair<UImage*, EWeaponType>> arrWeaponImg
    {
        TPair<UImage*, EWeaponType>{ FirstGunSlotImg,  FIRST },
        TPair<UImage*, EWeaponType>{ SecondGunSlotImg, SECOND },
        TPair<UImage*, EWeaponType>{ PistolSlotImg,    PISTOL },
        TPair<UImage*, EWeaponType>{ MeleeSlotImg,     MELEE },
        TPair<UImage*, EWeaponType>{ GrenadeSlotImg,   THROWABLE }
    };
    // 총 무기 다섯칸 중 어느거 선택했는지 확인
    for (auto item : arrWeaponImg)
    {
        auto weaponImg = item.Key;

        if (weaponImg->IsVisible() &&
            weaponImg->IsHovered())
            mSelectedWeaponIndex = item.Value;
    }
}

void UInventoryWeaponSlotUI::CheckForHoveredAttachmentSlot()
{
    // 첫번째/두번째 무기 부속품 UI 슬롯 확인
    for (int i = 0; i < mkTotalGunAttachmentUI; i++)
    {
        auto firstAttachmentBorder  = mArrFirstGunAttachmentBorder[i];
        auto secondAttachmentBorder = mArrSecondGunAttachmentBorder[i];

    	if (firstAttachmentBorder->IsHovered())
        {
            firstAttachmentBorder->SetBrushColor(mkHighlightColor);
            mSelectedWeaponIndex = FIRST;
            
        }
        else
            firstAttachmentBorder->SetBrushColor(mkNormalColor);

        if (secondAttachmentBorder->IsHovered())
        {
            secondAttachmentBorder->SetBrushColor(mkHighlightColor);
            mSelectedWeaponIndex = SECOND;
        }
        else
            secondAttachmentBorder->SetBrushColor(mkNormalColor);

        // 세번째 무기 부속품 UI 슬롯 확인
        if (i < mkTotalPistolAttachmentUI)
        {
            auto pistolAttachmentBorder = mArrPistolAttachmentBorder[i];

            if (pistolAttachmentBorder->IsHovered())
            {
                pistolAttachmentBorder->SetBrushColor(mkHighlightColor);
                mSelectedWeaponIndex = PISTOL;
            }
            else
                pistolAttachmentBorder->SetBrushColor(mkNormalColor);
        }

    }
}

void UInventoryWeaponSlotUI::UpdateHighlightImgPos()
{
    float sizeX = 0.f;
    UCanvasPanelSlot* p_canvasPanelSlot = nullptr;

    // 무기가 선택됐을 시 이미지 설정
    switch (mSelectedWeaponIndex)
    {
    case FIRST:     p_canvasPanelSlot = FirstGunCanvasPanel->AddChildToCanvas(HighlightImg);  sizeX = 545.f; break;
    case SECOND:    p_canvasPanelSlot = SecondGunCanvasPanel->AddChildToCanvas(HighlightImg); sizeX = 545.f; break;
    case PISTOL:    p_canvasPanelSlot = PistolCanvasPanel->AddChildToCanvas(HighlightImg);    sizeX = 545.f; break;
    case MELEE:     p_canvasPanelSlot = MeleeCanvasPanel->AddChildToCanvas(HighlightImg);     sizeX = 225.f; break;
    case THROWABLE: p_canvasPanelSlot = GrenadeCanvasPanel->AddChildToCanvas(HighlightImg);   sizeX = 225.f; break;
    }
    if (p_canvasPanelSlot)
    {
        p_canvasPanelSlot->SetSize(FVector2D(sizeX, 191.f));
        HighlightImg->SetVisibility(VISIBLE);
        HighlightImg->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.1f });
    }
}

void UInventoryWeaponSlotUI::ResetHighlightImg()
{
    // 선택 이미지 부모 새로 지정 및 무기 슬롯 이미지 설정
    switch (mSelectedWeaponIndex)
    {
    case FIRST:     FirstGunCanvasPanel->RemoveChild(HighlightImg);  break;
    case SECOND:    SecondGunCanvasPanel->RemoveChild(HighlightImg); break;
    case PISTOL:    PistolCanvasPanel->RemoveChild(HighlightImg);    break;
    case MELEE:     MeleeCanvasPanel->RemoveChild(HighlightImg);     break;
    case THROWABLE: GrenadeCanvasPanel->RemoveChild(HighlightImg);   break;
    }
    MainCanvasPanel->AddChildToCanvas(HighlightImg);
    HighlightImg->SetVisibility(HIDDEN);
    mSelectedWeaponIndex = EWeaponType::NONE;
    mbClicked = false;
}

int UInventoryWeaponSlotUI::GetAttachmentSlotIndex(FString AttachmentType) const
{
    int index = -1;

    if      (AttachmentType == "Scope" ||
             AttachmentType == "Sight" ||
             AttachmentType == "IRS")
             index = 0;

    else if (AttachmentType == "Stock"  ||
             AttachmentType == "StockA" ||
             AttachmentType == "StockB")
             index = 1;

    else if (AttachmentType == "Grip")
             index = 2;

    else if (AttachmentType == "Forend")
             index = 3;

    else if (AttachmentType == "Barrel")
             index = 4;

    return index;
}

void UInventoryWeaponSlotUI::SetSlotNull()
{
    if (mpWeaponManager)
    {
        mpWeaponManager->SetNull((EWeaponType)mDraggedWeaponIndex);
        ResetHighlightImg();
        mDraggedWeaponIndex = NONE;
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
        GrenadeSlotImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)pThrowable->WeaponType));
        GrenadeNameTxt->SetText(FText::FromString(pThrowable->WeaponData.Type));
    }
    for (int i = 0; i < arrThrowableWidget.Num(); i++)
        arrThrowableWidget[i]->SetVisibility(pThrowable ? VISIBLE : HIDDEN);
}

void UInventoryWeaponSlotUI::VerifyAttachmentSlot(ACoreAttachment* pAttachment)
{
    if (pAttachment &&
        mpWeaponManager)
    {
        auto attachmentData = pAttachment->WeaponAttachmentData;
        auto groupType      = attachmentData.GroupType;
        auto matchType      = attachmentData.WeaponMatchType;
        int index = GetAttachmentSlotIndex(groupType);

        // 여러 무기들이 착용 가능할 경우
        if (matchType == "All")
        {
            mArrFirstGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
            mArrSecondGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);

            if (index < mkTotalPistolAttachmentUI)
                mArrPistolAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
        }
        // 특정 여러 무기들이 착용 가능할 경우
        else
        {
            // , 기준으로 문자열을 잘라냄 
            auto arrString = UKismetStringLibrary::ParseIntoArray(matchType, ",");
            
            // 각 무기 타입 순회
            for (auto matchStr : arrString)
            {
                for (int i = 0; i < mArrWeapon.Num(); i++)
                {
                	if (auto p_gun = mArrWeapon[i])
                    {
                        auto data = p_gun->WeaponData;

                        // 타입 또는 무기 종료 일치할 시
                        if (data.Type      == matchStr ||
                            data.GroupType == matchStr)
                        {
                            // 선택 색상을 입힘
                            switch (i)
                            {
                            case 0: mArrFirstGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);  break;
                            case 1: mArrSecondGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor); break;
                            case 2: mArrPistolAttachmentBorder[index]->SetBrushColor(mkHighlightColor);    break;
                            }
                        }
                    }
                }
            }
        }
    }
    // 원상복구 시킴
    else
    {
        for (int i = 0; i < mkTotalGunAttachmentUI; i++)
        {
            // 첫번째 무기 뒷배경
            if (auto p_firstGunAttachmentBorder = mArrFirstGunAttachmentBorder[i])
                p_firstGunAttachmentBorder->SetBrushColor(mkNormalColor);

            // 두번째 무기 뒷배경
            if (auto p_secondGunAttachmentBorder = mArrSecondGunAttachmentBorder[i])
                p_secondGunAttachmentBorder->SetBrushColor(mkNormalColor);
        }
        for (int i = 0; i < mkTotalPistolAttachmentUI; i++)
        {
            // 세번째 무기 뒷배경
            if (auto p_pistolAttachmentBorder = mArrPistolAttachmentBorder[i])
                p_pistolAttachmentBorder->SetBrushColor(mkNormalColor);
        }
    }
}