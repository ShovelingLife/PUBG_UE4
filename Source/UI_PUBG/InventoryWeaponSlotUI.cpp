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

    pGameInstanceSubSystemUI = UGameInstance::GetSubsystem<UGameInstanceSubsystemUI>(GetWorld()->GetGameInstance());

    if (pGameInstanceSubSystemUI)
        pGameInstanceSubSystemUI->DeleVerifyAttachmentSlot.BindUFunction(this, "VerifyAttachmentSlot");
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

        SetWeaponSlotVisibility();
        UpdateInventoryWeaponUI();
        UpdateAttachmentSlot();
    }
    if (!mbClicked)
    {
        UpdateHighlightImgPos();
        CheckForHoveredWeaponSlot();
        CheckForHoveredAttachmentSlot();
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

    if (pGameInstanceSubSystemUI)
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
            p_weapon = p_weaponManager->GetWeaponByIndex(mSelectedWeaponIndex);
    }
    auto      p_slot = CreateWidget<UItemSlotUI>(GetWorld(), BP_itemSlotUI);
    FVector2D mousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (!p_slot ||
        !p_weapon ||
        mItemData.IsEmpty())
        return;

    // 슬롯 설정
    if (auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
        subGameInst->DeleSetTooltipVisibility.ExecuteIfBound(nullptr, Hidden);

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
        p_customOperation->bFromWeaponSlot = true;
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
    AWeaponManager* p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    if (!p_weaponManager ||
        !p_slot)
        return false;

    // 현재 선택하고 있는 인덱스 확인
    auto p_draggedWeapon = p_slot->pDraggedItem;

    // 맞지 않는 슬롯에 위치시킬 시
    if (p_weaponManager->IsWrongType(p_draggedWeapon, mSelectedWeaponIndex, p_customOperation->bFromWeaponSlot))
        return false;

    // 무기 선택
    ABaseInteraction* p_selectedWeapon = p_weaponManager->GetWeaponByIndex((ECurrentWeaponType)mSelectedWeaponIndex);
    
    if (p_weaponManager->Swap(p_draggedWeapon, p_selectedWeapon, mSelectedWeaponIndex) == -1)
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
        if (!p_customOperation->bFromInventoryList)
            p_slot->DeleSwapWeaponSlot.ExecuteIfBound(p_slot);

        else
            p_slot->DeleSwapInventoryExplosive.ExecuteIfBound(Cast<ACoreThrowableWeapon>(p_selectedWeapon), Cast<ACoreThrowableWeapon>(p_draggedWeapon));
    }
    p_customOperation->bFromInventoryList = false;
    p_customOperation->bFromWeaponSlot    = false;
    p_customOperation->pSlotUI            = p_slot;
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
        arrFirstGunWidget[i]->SetVisibility((mpWeaponManager->pFirstGun) ? Visible : Hidden);
        arrSecondGunWidget[i]->SetVisibility((mpWeaponManager->pSecondGun) ? Visible : Hidden);
        arrPistolWidget[i]->SetVisibility((mpWeaponManager->pPistol) ? Visible : Hidden);
    }
    // 기타 무기 설정
    for (int i = 0; i < 3; i++)
         arrMeleeWidget[i]->SetVisibility((mpWeaponManager->pMelee) ? Visible : Hidden);
}

void UInventoryWeaponSlotUI::UpdateAttachmentSlot()
{
    // 총기 부착물 UI 갱신
    for (int i = 0; i < mkTotalGunAttachmentUI; i++)
    {
        // 첫번째 무기
        {
            TArray<UTexture*> arrAttachmentTex;
            ACoreBarrel*      p_barrel = nullptr;
            ACoreGrip*        p_grip   = nullptr;
            ACoreForend*      p_forend = nullptr;
            ACoreStock*       p_stock  = nullptr;
            ACoreSight*       p_scope  = nullptr;

            // 텍스처 설정
            if (auto p_firstGun = mpWeaponManager->pFirstGun)
            {
                p_barrel = p_firstGun->CurrentBarrel;
                p_grip   = p_firstGun->CurrentGrip;
                p_forend = p_firstGun->CurrentForend;
                p_stock  = p_firstGun->CurrentStock;
                p_scope  = p_firstGun->CurrentSight;
            }
            arrAttachmentTex.Add((p_barrel) ? p_barrel->CurrentItemTex : nullptr);
            arrAttachmentTex.Add((p_grip)   ? p_grip->CurrentItemTex   : nullptr);
            arrAttachmentTex.Add((p_forend) ? p_forend->CurrentItemTex : nullptr);
            arrAttachmentTex.Add((p_stock)  ? p_stock->CurrentItemTex  : nullptr);
            arrAttachmentTex.Add((p_scope)  ? p_scope->CurrentItemTex  : nullptr);

            // 뒷배경
            if (auto p_firstGunAttachmentBorder = mArrFirstGunAttachmentBorder[i])
                p_firstGunAttachmentBorder->SetVisibility((mpWeaponManager->pFirstGun) ? Visible : Hidden);

            // 아이템 UI
            if (auto p_firstGunAttachmentUI = mArrFirstGunAttachmentUI[i])
                p_firstGunAttachmentUI->ItemImg->SetBrushFromTexture(Cast<UTexture2D>(arrAttachmentTex[i]));
        }
        // 두번째 무기
        {
            TArray<UTexture*> arrAttachmentTex;
            ACoreBarrel*      p_barrel = nullptr;
            ACoreGrip*        p_grip   = nullptr;
            ACoreForend*      p_forend = nullptr;
            ACoreStock*       p_stock  = nullptr;
            ACoreSight*       p_scope  = nullptr;

            // 텍스처 설정
            if (auto p_secondGun = mpWeaponManager->pSecondGun)
            {
                p_barrel = p_secondGun->CurrentBarrel;
                p_grip   = p_secondGun->CurrentGrip;
                p_forend = p_secondGun->CurrentForend;
                p_stock  = p_secondGun->CurrentStock;
                p_scope  = p_secondGun->CurrentSight;
            }
            arrAttachmentTex.Add((p_barrel) ? p_barrel->CurrentItemTex : nullptr);
            arrAttachmentTex.Add((p_grip)   ? p_grip->CurrentItemTex   : nullptr);
            arrAttachmentTex.Add((p_forend) ? p_forend->CurrentItemTex : nullptr);
            arrAttachmentTex.Add((p_stock)  ? p_stock->CurrentItemTex  : nullptr);
            arrAttachmentTex.Add((p_scope)  ? p_scope->CurrentItemTex  : nullptr);

            // 뒷배경
            if (auto p_secondGunAttachmentBorder = mArrSecondGunAttachmentBorder[i])
                p_secondGunAttachmentBorder->SetVisibility((mpWeaponManager->pSecondGun) ? Visible : Hidden);

            // 아이템 UI
            if (auto p_secondGunAttachmentUI = mArrSecondGunAttachmentUI[i])
                p_secondGunAttachmentUI->ItemImg->SetBrushFromTexture(Cast<UTexture2D>(arrAttachmentTex[i]));
        }
    }
    // 세번쨰 무기
    for (int i = 0; i < mkTotalPistolAttachmentUI; i++)
    {
        TArray<UTexture*> arrAttachmentTex;
        ACoreBarrel*      p_barrel = nullptr;
        ACoreForend*      p_forend = nullptr;
        ACoreSight*       p_scope  = nullptr;

        // 텍스처 설정
        if (auto p_pistol = mpWeaponManager->pFirstGun)
        {
            p_barrel = p_pistol->CurrentBarrel;
            p_forend = p_pistol->CurrentForend;
            p_scope  = p_pistol->CurrentSight;
        }
        arrAttachmentTex.Add((p_barrel) ? p_barrel->CurrentItemTex : nullptr);
        arrAttachmentTex.Add((p_forend) ? p_forend->CurrentItemTex : nullptr);
        arrAttachmentTex.Add((p_scope)  ? p_scope->CurrentItemTex  : nullptr);

        // 뒷배경
        if (auto p_pistolAttachmentBorder = mArrPistolAttachmentBorder[i])
            p_pistolAttachmentBorder->SetVisibility((mpWeaponManager->pPistol) ? Visible : Hidden);

        // 아이템 UI
        if (auto p_pistolAttachmentUI = mArrFirstGunAttachmentUI[i])
            p_pistolAttachmentUI->ItemImg->SetBrushFromTexture(Cast<UTexture2D>(arrAttachmentTex[i]));
    }
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
    TArray<Chaos::Pair<UImage*, ECurrentWeaponType>> arrWeaponImg
    {
        Chaos::MakePair<UImage*, ECurrentWeaponType>(FirstGunSlotImg,  ECurrentWeaponType::FIRST),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(SecondGunSlotImg, ECurrentWeaponType::SECOND),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(PistolSlotImg,    ECurrentWeaponType::PISTOL),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(MeleeSlotImg,     ECurrentWeaponType::MELEE),
        Chaos::MakePair<UImage*, ECurrentWeaponType>(GrenadeSlotImg,   ECurrentWeaponType::THROWABLE)
    };
    // 총 무기 다섯칸 중 어느거 선택했는지 확인
    for (auto item : arrWeaponImg)
    {
        auto weaponImg = item.First;

        if (weaponImg->IsVisible() &&
            weaponImg->IsHovered())
            mSelectedWeaponIndex = item.Second;
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
            mSelectedWeaponIndex = ECurrentWeaponType::FIRST;
            
        }
        else
            firstAttachmentBorder->SetBrushColor(mkNormalColor);

        if (secondAttachmentBorder->IsHovered())
        {
            secondAttachmentBorder->SetBrushColor(mkHighlightColor);
            mSelectedWeaponIndex = ECurrentWeaponType::SECOND;
        }
        else
            secondAttachmentBorder->SetBrushColor(mkNormalColor);
    }
    // 세번째 무기 부속품 UI 슬롯 확인
    for (int i = 0; i < mkTotalPistolAttachmentUI; i++)
    {
        auto pistolAttachmentBorder = mArrPistolAttachmentBorder[i];

        if (pistolAttachmentBorder->IsHovered())
        {
            pistolAttachmentBorder->SetBrushColor(mkHighlightColor);
            mSelectedWeaponIndex = ECurrentWeaponType::PISTOL;
        }
        else
            pistolAttachmentBorder->SetBrushColor(mkNormalColor);
    }
}

void UInventoryWeaponSlotUI::UpdateHighlightImgPos()
{
    float sizeX = 0.f;
    UCanvasPanelSlot* p_canvasPanelSlot = nullptr;

    // 무기가 선택됐을 시 이미지 설정
    switch (mSelectedWeaponIndex)
    {
    case ECurrentWeaponType::FIRST:     p_canvasPanelSlot = FirstGunCanvasPanel->AddChildToCanvas(HighlightImg);  sizeX = 545.f; break;
    case ECurrentWeaponType::SECOND:    p_canvasPanelSlot = SecondGunCanvasPanel->AddChildToCanvas(HighlightImg); sizeX = 545.f; break;
    case ECurrentWeaponType::PISTOL:    p_canvasPanelSlot = PistolCanvasPanel->AddChildToCanvas(HighlightImg);    sizeX = 545.f; break;
    case ECurrentWeaponType::MELEE:     p_canvasPanelSlot = MeleeCanvasPanel->AddChildToCanvas(HighlightImg);     sizeX = 225.f; break;
    case ECurrentWeaponType::THROWABLE: p_canvasPanelSlot = GrenadeCanvasPanel->AddChildToCanvas(HighlightImg);   sizeX = 225.f; break;
    }
    if (p_canvasPanelSlot)
    {
        p_canvasPanelSlot->SetSize(FVector2D(sizeX, 191.f));
        HighlightImg->SetVisibility(Visible);
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
    case ECurrentWeaponType::MELEE:     MeleeCanvasPanel->RemoveChild(HighlightImg);     break;
    case ECurrentWeaponType::THROWABLE: GrenadeCanvasPanel->RemoveChild(HighlightImg);   break;
    }
    MainCanvasPanel->AddChildToCanvas(HighlightImg);
    HighlightImg->SetVisibility(Hidden);
    mSelectedWeaponIndex = ECurrentWeaponType::NONE;
    mbClicked = false;
}

int UInventoryWeaponSlotUI::GetAttachmentSlotIndex(FString AttachmentType)
{
    int index = -1;

    if      (AttachmentType == "Scope" ||
             AttachmentType == "Sight" ||
             AttachmentType == "IRS")
             index = 0;

    else if (AttachmentType == "Stock" ||
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
        mpWeaponManager->SetNull((ECurrentWeaponType)mDraggedWeaponIndex);
        ResetHighlightImg();
        mDraggedWeaponIndex = ECurrentWeaponType::NONE;
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
        arrThrowableWidget[i]->SetVisibility(pThrowable ? Visible : Hidden);
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

        // 전체 무기 착용 가능함
        if (matchType == "All")
        {
            mArrFirstGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
            mArrSecondGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);

            if (index < mkTotalPistolAttachmentUI)
                mArrPistolAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
        }
        else
        {
            // , 기준으로 문자열을 잘라냄 
            auto arrString = UKismetStringLibrary::ParseIntoArray(matchType, ",");

            for (auto matchStr : arrString)
            {
                // 첫번째 총기
                if (auto p_firstGun = mpWeaponManager->pFirstGun)
                {
                    auto data = p_firstGun->WeaponData;

                    if (data.Type      == matchStr ||
                        data.GroupType == matchStr)
                        mArrFirstGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
                }
                // 두번째 총기
                if (auto p_secondGun = mpWeaponManager->pSecondGun)
                {
                    auto data = p_secondGun->WeaponData;

                    if (data.Type      == matchStr ||
                        data.GroupType == matchStr)
                        mArrSecondGunAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
                }
                // 세번째 총기
                if (auto p_pistol = mpWeaponManager->pPistol)
                {
                    if (p_pistol->WeaponData.Type == matchStr)
                        mArrPistolAttachmentBorder[index]->SetBrushColor(mkHighlightColor);
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