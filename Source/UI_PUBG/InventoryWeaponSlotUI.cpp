#include "InventoryWeaponSlotUI.h"
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
}

void UInventoryWeaponSlotUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);

    if (!mpWeaponManager)
        mpWeaponManager = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetWeaponManager();

    if (!mpUI_manager)
        mpUI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    CheckForHoveredWeaponSlot();
    UpdateVisibility();
    UpdateInventoryWeaponUI();
    UpdateHighlightImgPos(); 
}

void UInventoryWeaponSlotUI::NativeOnMouseLeave(const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseLeave(_InMouseEvent);

    if (!mbClicked)
        ResetHighlightImg();
}

FReply UInventoryWeaponSlotUI::NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseButtonDown(_InGeometry, _InMouseEvent);
    
    // 왼쪽 클릭 / 마우스 커서 변경
    if(_InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        GetIemData(mpWeaponManager->GetWeaponByIndex(mSelectedWeaponIndex));
        HighlightImg->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.35f });
        mbClicked           = true;
        mDraggedWeaponIndex = mSelectedWeaponIndex;
    }
    // 오른쪽 클릭 / 맵에다가 무기를 버림
    else if (_InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        mpWeaponManager->Drop(mSelectedWeaponIndex);
        ResetHighlightImg();
    }
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(_InMouseEvent, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UInventoryWeaponSlotUI::NativeOnMouseButtonUp(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseButtonUp(_InGeometry, _InMouseEvent);
    mItemData.Reset();
    ResetHighlightImg();
    return FReply::Handled();
}
void UInventoryWeaponSlotUI::NativeOnDragDetected(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent, UDragDropOperation*& _OutOperation)
{
    Super::NativeOnDragDetected(_InGeometry, _InMouseEvent, _OutOperation);

    ABaseInteraction* p_weapon = mpWeaponManager->GetWeaponByIndex(mSelectedWeaponIndex);
    auto              p_slot   = CreateWidget<UItemSlotUI>(GetWorld(), BP_itemSlotUI);
    FVector2D         mousePos = _InGeometry.AbsoluteToLocal(_InMouseEvent.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (!p_slot   ||
        !p_weapon ||
        mItemData.IsEmpty())
        return;

    // 슬롯 설정
    auto subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();

    if (subGameInst)
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
    _OutOperation = p_customOperation;
}

bool UInventoryWeaponSlotUI::NativeOnDrop(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDrop(_InGeometry, _InDragDropEvent, _InOperation);
    auto p_customOperation = Cast<UCustomDragDropOperation>(_InOperation);
    auto p_slot = p_customOperation->pSlotUI;
    p_customOperation->bFromList = false;

    if (!p_slot)
        return false;

    // 현재 선택하고 있는 인덱스 확인
    auto p_draggedWeapon  = p_slot->pDraggedItem;

    if (!p_draggedWeapon)
        return false;

    //p_slot->TooltipVisibility = ESlateVisibility::Hidden;

    // 총기가 수류탄 또는 근접무기 슬롯에 위치시킬 시
    if ((int)mSelectedWeaponIndex < 4 &&
        mpWeaponManager->IsDuplicated(p_draggedWeapon, ECurrentWeaponType::NONE))
        return false;

    // 1 2번 슬롯 같은 경우 무기가 중복 여부 같은 슬롯 내에는 발생하지 않음
    if(p_customOperation->bFromList)
    {
        if (mpWeaponManager->IsDuplicated(p_draggedWeapon, mSelectedWeaponIndex))
            return false;
    }
    // 무기 선택
    ABaseInteraction* p_selectedWeapon =  mpWeaponManager->GetWeaponByIndex((ECurrentWeaponType)mSelectedWeaponIndex);
    
    if (!p_selectedWeapon) // 무기 장착, 해당 무기 슬롯이 비어있음
        GetIemData(p_draggedWeapon);

    else // 무기 교체
        GetIemData(p_selectedWeapon);

    if (!mpWeaponManager->Swap(p_draggedWeapon, p_selectedWeapon, mSelectedWeaponIndex))
        return false;

    // 무기 배치 및 UI 설정이 완료 되었다면 초기화
    if (!p_selectedWeapon)
        p_slot->DeleSetSlotNull.ExecuteIfBound();

    else
        p_slot->DeleSwapWeaponSlot.ExecuteIfBound(p_slot);

    p_slot->ItemData     = mItemData;
    p_slot->pDraggedItem = p_selectedWeapon;
    return true;
}

void UInventoryWeaponSlotUI::NativeOnDragEnter(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDragEnter(_InGeometry, _InDragDropEvent, _InOperation);
    ResetHighlightImg();
}

void UInventoryWeaponSlotUI::NativeOnDragLeave(const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDragLeave(_InDragDropEvent, _InOperation);
    ResetHighlightImg();
}

void UInventoryWeaponSlotUI::NativeOnDragCancelled(const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDragCancelled(_InDragDropEvent, _InOperation);
    ResetHighlightImg();
}

bool UInventoryWeaponSlotUI::NativeOnDragOver(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDragOver(_InGeometry, _InDragDropEvent, _InOperation);    
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
    // 첫번째 무기
    FirstGunSlotImg->SetVisibility(            (mpWeaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FirstGunNameTxt->SetVisibility(            (mpWeaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FirstGunBulletTypeTxt->SetVisibility(      (mpWeaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FirstGunCurrentMagazineTxt->SetVisibility( (mpWeaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FirstGunMaxMagazineTxt->SetVisibility(     (mpWeaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FirstGunNumberBackground->SetVisibility(   (mpWeaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 두번째 무기
    SecondGunSlotImg->SetVisibility(           (mpWeaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    SecondGunNameTxt->SetVisibility(           (mpWeaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    SecondGunBulletTypeTxt->SetVisibility(     (mpWeaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    SecondGunCurrentMagazineTxt->SetVisibility((mpWeaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    SecondGunMaxMagazineTxt->SetVisibility(    (mpWeaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    SecondGunNumberBackground->SetVisibility(  (mpWeaponManager->pSecondGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 세번째 무기
    PistolSlotImg->SetVisibility(              (mpWeaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    PistolNameTxt->SetVisibility(              (mpWeaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    PistolBulletTypeTxt->SetVisibility(        (mpWeaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    PistolCurrentMagazineTxt->SetVisibility(   (mpWeaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    PistolMaxMagazineTxt->SetVisibility(       (mpWeaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    PistolNumberBackground->SetVisibility(     (mpWeaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 네번째 무기
    MeleeSlotImg->SetVisibility(               (mpWeaponManager->pMelee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    MeleeNameTxt->SetVisibility(               (mpWeaponManager->pMelee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    MeleeNumberBackground->SetVisibility(      (mpWeaponManager->pMelee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 다섯번째 무기
    GrenadeSlotImg->SetVisibility(             (mpWeaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    GrenadeNameTxt->SetVisibility(             (mpWeaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    GrenadeNumberBackground->SetVisibility(    (mpWeaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInventoryWeaponSlotUI::UpdateInventoryWeaponUI()
{
    if (!mpUI_manager ||
        !mpWeaponManager)
        return;

    // 첫번째 무기
    if(auto p_firstGun = mpWeaponManager->pFirstGun)
    {
        auto weaponData = p_firstGun->WeaponData;
        FirstGunSlotImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapInventoryGunWeaponTex[(int)p_firstGun->WeaponType]));
        FirstGunNameTxt->SetText(FText::FromString(weaponData.Type));
        FirstGunBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
    }
    // 두번째 무기
    if (auto p_secondGun = mpWeaponManager->pSecondGun)
    {
        auto weaponData = p_secondGun->WeaponData;
        SecondGunSlotImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapInventoryGunWeaponTex[(int)p_secondGun->WeaponType]));
        SecondGunNameTxt->SetText(FText::FromString(weaponData.Type));
        SecondGunBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
    }
    // 세번째 무기
    if (auto p_pistol = mpWeaponManager->pPistol)
    {
        auto weaponData = p_pistol->WeaponData;
        PistolSlotImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapInventoryGunWeaponTex[(int)mpWeaponManager->pPistol->WeaponType]));
        PistolNameTxt->SetText(FText::FromString(weaponData.Type));
        PistolBulletTypeTxt->SetText(FText::FromString(weaponData.BulletType));
    }
    // 네번째 무기
    if (auto p_throwable = mpWeaponManager->pThrowable)
    {
        auto weaponData = p_throwable->WeaponData;
        MeleeSlotImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapInventoryGunWeaponTex[(int)mpWeaponManager->pThrowable->WeaponType]));
        MeleeNameTxt->SetText(FText::FromString(weaponData.Type));
    }
    // 다섯번째 무기
    if (auto p_melee = mpWeaponManager->pMelee)
    {
        auto weaponData = p_melee->WeaponData;
        GrenadeSlotImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapInventoryGunWeaponTex[(int)mpWeaponManager->pMelee->WeaponType]));
        GrenadeNameTxt->SetText(FText::FromString(weaponData.Type));
    }
}

void UInventoryWeaponSlotUI::CheckForHoveredWeaponSlot()
{
    if (mbClicked)
        return;

    // 선택된 무기 인덱스 구함
    if      (FirstGunSlotImg->GetVisibility() == ESlateVisibility::Visible &&
             FirstGunSlotImg->IsHovered())
             mSelectedWeaponIndex = ECurrentWeaponType::FIRST;

    else if (SecondGunSlotImg->GetVisibility() == ESlateVisibility::Visible &&
             SecondGunSlotImg->IsHovered())
             mSelectedWeaponIndex = ECurrentWeaponType::SECOND;

    else if (PistolSlotImg->GetVisibility() == ESlateVisibility::Visible &&
             PistolSlotImg->IsHovered())
             mSelectedWeaponIndex = ECurrentWeaponType::PISTOL;

    else if (MeleeSlotImg->GetVisibility() == ESlateVisibility::Visible &&
             MeleeSlotImg->IsHovered())
             mSelectedWeaponIndex = ECurrentWeaponType::MELEE;

    else if (GrenadeSlotImg->GetVisibility() == ESlateVisibility::Visible &&
             GrenadeSlotImg->IsHovered())
             mSelectedWeaponIndex = ECurrentWeaponType::THROWABLE;
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
        p_canvasPanelSlot->SetSize(FVector2D(sizeX, 195.f));
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

void UInventoryWeaponSlotUI::GetIemData(ABaseInteraction* _pWeapon)
{
    switch (mSelectedWeaponIndex)
    {
    // 총기류
    case ECurrentWeaponType::FIRST: 
    case ECurrentWeaponType::SECOND: 
    case ECurrentWeaponType::PISTOL:

        if (auto p_tmp_gun = Cast<ACoreWeapon>(_pWeapon))
            mItemData = FsSlotItemData(p_tmp_gun->ObjectType, "", (int)p_tmp_gun->WeaponType);

        break;

    // 근접 무기
    case ECurrentWeaponType::MELEE:

        if (auto p_tmp_melee = Cast<ACoreMeleeWeapon>(_pWeapon))
            mItemData = FsSlotItemData(p_tmp_melee->ObjectType, "", (int)p_tmp_melee->WeaponType);

        break;

    // 투척류 무기
    case ECurrentWeaponType::THROWABLE:

        if (auto p_tmp_throwable = Cast<ACoreThrowableWeapon>(_pWeapon))
            mItemData = FsSlotItemData(p_tmp_throwable->ObjectType, "", (int)p_tmp_throwable->WeaponType);

        break;
    }
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
    if (mpWeaponManager)
    {
        mpWeaponManager->SetNull((ECurrentWeaponType)mDraggedWeaponIndex);
        ResetHighlightImg();
        mDraggedWeaponIndex = ECurrentWeaponType::NONE;
        mItemData.Reset();
    }
}