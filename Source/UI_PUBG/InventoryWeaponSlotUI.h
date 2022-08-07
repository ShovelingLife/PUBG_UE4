/**
 * \file Inventory_Weapon_Slot_UI.h
 *
 * \brief 인벤토리 무기 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Events.h"
#include "ItemSlotUI.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "InventoryWeaponSlotUI.generated.h"

class AUI_manager;
class ACoreWeapon;
class AWeaponManager;
class ACoreAttachment;
class ACoreThrowableWeapon;
class UItemSlotUI;
class UBorder;
class UCanvasPanel;
class UImage;
class UTextBlock;

UCLASS()
class UI_PUBG_API UInventoryWeaponSlotUI : public UUserWidget
{
    GENERATED_BODY()
        
using enum EWeaponType;

private:
    // 뒷배경 색상 관련
    const FLinearColor mkHighlightColor = FLinearColor(FColor::White),
                       mkNormalColor    = FLinearColor(0.f, 0.f, 0.f, 0.25f);

    /** \brief 현재 쓰여지는 변수 */
    TArray<ACoreWeapon*> mArrWeapon
    {
        nullptr,
        nullptr,
        nullptr
    };
    AWeaponManager* mpWeaponManager = nullptr;
    FsSlotItemData  mItemData;

    // 부속품 UI 관련
    TArray<UBorder*>     mArrFirstGunAttachmentBorder;
    TArray<UBorder*>     mArrSecondGunAttachmentBorder;
    TArray<UBorder*>     mArrPistolAttachmentBorder;
    TArray<UItemSlotUI*> mArrFirstGunAttachmentUI;
    TArray<UItemSlotUI*> mArrSecondGunAttachmentUI;
    TArray<UItemSlotUI*> mArrPistolAttachmentUI;
    const int mkTotalGunAttachmentUI    = 5;
    const int mkTotalPistolAttachmentUI = 3;

    // 플레이어 상호작용 하고있는 변수 관련
    EWeaponType mSelectedWeaponIndex;
    EWeaponType mDraggedWeaponIndex;
    bool        mbClicked = false;

public:
    /** \brief 선택 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       HighlightImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* MainCanvasPanel;

    /** \brief 1번 슬롯 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* FirstGunCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      FirstGunNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunBulletTypeTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunCurrentMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   FirstGunMaxMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       FirstGunSlotImg;

    /** \brief 1번 슬롯 부속품 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     FirstGunScopeSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     FirstGunStockSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     FirstGunGripSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     FirstGunMagazineSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     FirstGunMuzzleSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* FirstGunScopeSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* FirstGunStockSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* FirstGunGripSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* FirstGunMagazineSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* FirstGunMuzzleSlotUI;

    /** \brief 2번 슬롯 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* SecondGunCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      SecondGunNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunBulletTypeTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunCurrentMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   SecondGunMaxMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       SecondGunSlotImg;

    /** \brief 2번 슬롯 부속품 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     SecondGunScopeSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     SecondGunStockSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     SecondGunGripSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     SecondGunMagazineSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     SecondGunMuzzleSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* SecondGunScopeSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* SecondGunStockSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* SecondGunGripSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* SecondGunMagazineSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* SecondGunMuzzleSlotUI;

    /** \brief 3번 슬롯 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* PistolCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      PistolNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolBulletTypeTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolCurrentMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   PistolMaxMagazineTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       PistolSlotImg;

    /** \brief 3번 슬롯 부속품 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     PistolScopeSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     PistolMagazineSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     PistolMuzzleSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* PistolScopeSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* PistolMagazineSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* PistolMuzzleSlotUI;

    /** \brief 4번 슬롯 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* MeleeCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      MeleeNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   MeleeNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       MeleeSlotImg;

    /** \brief 5번 슬롯 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* GrenadeCanvasPanel;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*      GrenadeNumberBackground;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   GrenadeNameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       GrenadeSlotImg;

    /** \brief 아이템 슬롯 BP */
    UPROPERTY(EditDefaultsOnly, Category = Item_slot) TSubclassOf<UItemSlotUI> BP_itemSlotUI;

    UPROPERTY() class UGameInstanceSubsystemUI* pGameInstanceSubSystemUI = nullptr;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

    /**
      * \brief 마우스가 UI를 벗어날 시 선택 이미지 초기화
      * \param _in_mouse_event 마우스 이벤트
     */
    virtual void NativeOnMouseLeave(const FPointerEvent& _InMouseEvent) override;

    /**
      * \brief 좌클릭 시 (선택된 UI에 맞게끔 슬롯 변환 > 드래그) / 우클릭 시 맵에 드롭
      * 선택 이미지 초기화
      * \param _InGeometry UI 정보
      * \param _InMouseEvent 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;

    /**
      * \brief 마우스 버튼 땠을 시 선택 이미지 및 클릭된 UI 정보 삭제
      * \param _in_geometry UI 정보 \param _in_mouse_event 마우스 이벤트
      * \return FReply 마우스 처리 이벤트
     */
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, class UDragDropOperation*& InOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, class UDragDropOperation* InOperation) override;

    virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    // 무기 아이콘 및 슬롯 관한 함수
    void InitSettings();

    /** \brief UI 상태 업데이트 */
    void SetWeaponSlotVisibility();

    void UpdateAttachmentSlot();

    TArray<UTexture*> GetAttachmentTexArr(ACoreWeapon* pWeapon) const;

    void UpdateInventoryWeaponUI();

    void CheckForHoveredWeaponSlot();

    void CheckForHoveredAttachmentSlot();

    // ------- 선택하고 있는 UI 관련 -------
    // 
    // 갱신
    void UpdateHighlightImgPos();

    void ResetHighlightImg();

    // 문자열에 따라 해당되는 부속품 슬롯 인덱스를 반환
    int GetAttachmentSlotIndex(FString AttachmentType) const;

public:
    // 슬롯 초기화
    UFUNCTION() void SetSlotNull();

    UFUNCTION() void UpdateThrowable(ACoreThrowableWeapon* pGrenade);

    UFUNCTION() void VerifyAttachmentSlot(ACoreAttachment* pAttachment);
};