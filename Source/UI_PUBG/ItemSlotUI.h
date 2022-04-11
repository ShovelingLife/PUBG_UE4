/**
 * \file Item_Slot_UI.h
 *
 * \brief 아이템 슬롯 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SlotItemData.h"
#include "ItemSlotUI.generated.h" 

class UItemSlotUI;

DECLARE_DELEGATE_OneParam(FDeleCheckForSlot, UObject*)
DECLARE_DELEGATE_OneParam(FDeleSwapWeaponSlot, UItemSlotUI*)
DECLARE_DELEGATE(FDeleSetSlotNull)

class ABaseInteraction;
class UTooltipUI;
class UImage;
class UTextBlock;
class USizeBox;
class UBorder;
class UHorizontalBox;

UCLASS()
class UI_PUBG_API UItemSlotUI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
private:
    bool mbShouldCheckTooltipUI = false;

public:
    /** \brief 현재 쓰여지는 변수 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UHorizontalBox* MainHorizontalBox;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*       BackgroundSizeBox;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*        ItemBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*         ItemImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*     NameTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*     CountTxt;

    UPROPERTY() ABaseInteraction* pDraggedItem;
    FDeleCheckForSlot             DeleCheckForSlot;
    FDeleSwapWeaponSlot           DeleSwapWeaponSlot;
    FDeleSetSlotNull              DeleSetSlotNull;
    FsSlotItemData                ItemData;

protected:    
    virtual void NativeConstruct() override;

    /**
      * \brief 프레임마다 호출
      * \param InGeometry UI 정보 
      * \param DeltaTime 프레임 초
     */
    virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;

    /**
      * \brief 리스트에 아이템 설정 시 호출
      * \param pObj 슬롯 오브젝트
     */
    virtual void NativeOnListItemObjectSet(UObject* pObj);

public:
    /**
      * \brief 슬롯 UI를 커서 중앙 지점에 설정
      * \param Pos 슬롯 UI 위치 
     */
    void SetAsCursor(FVector2D Pos);
};