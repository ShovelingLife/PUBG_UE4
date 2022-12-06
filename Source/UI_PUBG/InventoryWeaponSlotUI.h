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
class UWeaponSocketUI;

class UBorder;
class UCanvasPanel;
class UImage;
class UTextBlock;
struct FPointerEvent;

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
    TArray<UCanvasPanel*> mArrCanvasPanel;
    AWeaponManager*       mpWeaponManager = nullptr;
    FPointerEvent         mCurrentPointerEvent;
    FsSlotItemData        mItemData;

    // 부속품 UI 관련
    TArray<UWeaponSocketUI*> mArrFirstGunAttachment;
    TArray<UWeaponSocketUI*> mArrSecondGunAttachment;
    TArray<UWeaponSocketUI*> mArrPistolAttachment;

    // 플레이어 상호작용 하고있는 변수 관련
    EWeaponType mSelectedWeaponType, mDraggedWeaponType;
    bool        mbClicked = false;

    UPROPERTY(EditAnywhere) 
    TSubclassOf<UItemSlotUI> BP_ItemSlotUI;

public:
    /** \brief 선택 UI 관련 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* HighlightImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UCanvasPanel* MainCanvasPanel;


    // ------- 1번 슬롯 UI 관련 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UCanvasPanel* FirstGunCanvasPanel;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UBorder* FirstGunNumberBackground;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* FirstGunNameTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* FirstGunBulletTypeTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* FirstGunCurrentMagazineTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* FirstGunMaxMagazineTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* FirstGunSlotImg;

    // ------- 1번 슬롯 부속품 관련 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* FirstGunScopeSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UWeaponSocketUI* FirstGunStockSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* FirstGunGripSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* FirstGunMagazineSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* FirstGunMuzzleSlot;


    // ------- 2번 슬롯 UI 관련 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UCanvasPanel* SecondGunCanvasPanel;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UBorder* SecondGunNumberBackground;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* SecondGunNameTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* SecondGunBulletTypeTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* SecondGunCurrentMagazineTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* SecondGunMaxMagazineTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* SecondGunSlotImg;

    // ------- 2번 슬롯 부속품 관련 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* SecondGunScopeSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* SecondGunStockSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* SecondGunGripSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* SecondGunMagazineSlot;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UWeaponSocketUI* SecondGunMuzzleSlot;


    // ------- 3번 슬롯 UI 관련 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UCanvasPanel* PistolCanvasPanel;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UBorder* PistolNumberBackground;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* PistolNameTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* PistolBulletTypeTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* PistolCurrentMagazineTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* PistolMaxMagazineTxt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* PistolSlotImg;

    /** \brief 3번 슬롯 부속품 관련 */

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UWeaponSocketUI* PistolScopeSlot;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UWeaponSocketUI* PistolMagazineSlot;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UWeaponSocketUI* PistolMuzzleSlot;

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

    UPROPERTY() class UGameInstanceSubsystemUI* pGameInstanceSubSystemUI = nullptr;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

    // 좌클릭 시 (선택된 UI에 맞게끔 슬롯 변환 > 드래그) / 우클릭 시 맵에 드롭
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 마우스 버튼 땠을 시 선택 이미지 및 클릭된 UI 정보 삭제
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 마우스가 UI를 벗어날 시 선택 이미지 초기화
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, class UDragDropOperation*& InOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, class UDragDropOperation* InOperation) override;

    virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    void InitWeaponSlot();

    /** \brief UI 상태 업데이트 */
    void UpdateWeaponSlotVisibility();

    void CheckForHoveredSlot();

    TArray<UTexture*> GetAttachmentTexArr(ACoreWeapon* pWeapon) const;

    void UpdateInventoryWeaponUI();

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
};