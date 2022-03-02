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

/**
  * \brief 아이템 슬롯 구조체
 */
USTRUCT()
struct FsSlotItemData
{
    GENERATED_BODY()

public:
    FString Name       = "";
    int     ImageIndex = 0;
    int     Count      = 0;

public:
    FsSlotItemData() = default;

    /**
      * \brief 기본 생성자 아이템 명칭 / 이미지 인덱스 / 개수 (UI매니저)
      * \param _name 아이템 명칭 \param _image_index UI 이미지 인덱스 \param _count 개수
     */
    FsSlotItemData(FString _Name, int _ImageIndex, int _Count = 1) : Name(_Name), ImageIndex(_ImageIndex), Count(_Count) { }

    bool operator==(FsSlotItemData& _Other)
    {
        return this->Name       == _Other.Name       &&
               this->ImageIndex == _Other.ImageIndex &&
               this->Count      == _Other.Count;
    }

    bool operator!=(FsSlotItemData& _Other) { return !(*this == _Other); }

public:
    void Reset()
    {
        this->Name        = "";
        this->ImageIndex = 0;
        this->Count       = 0;
    }

    bool Is_empty()
    {
        return this->Name       == "" &&
               this->ImageIndex == 0  &&
               this->Count      == 0; 
    }
};

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
      * \param _geometry UI 정보 \param _delta_time 프레임 초
     */
    virtual void NativeTick(const FGeometry& _geometry, float _delta_time) override;

    /**
      * \brief 리스트에 아이템 설정 시 호출
      * \param UObject 슬롯 오브젝트
     */
    virtual void NativeOnListItemObjectSet(UObject* _p_obj);

public:
    /**
      * \brief 슬롯 UI를 커서 중앙 지점에 설정
      * \param _Pos 슬롯 UI 위치 
     */
    void SetAsCursor(FVector2D _Pos);
};