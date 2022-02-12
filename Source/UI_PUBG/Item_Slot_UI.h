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
#include "Item_Slot_UI.generated.h" 

DECLARE_DELEGATE_OneParam(FDele_check_for_slot, UObject*)
DECLARE_DELEGATE(FDele_set_weapon_slot_null)

class UImage;
class UTextBlock;
class USizeBox;
class UBorder;
class UHorizontalBox;

/**
  * \brief 아이템 슬롯 구조체
 */
USTRUCT()
struct Fs_slot_item_data
{
    GENERATED_BODY()

public:
    FString name = "";
    int     image_index = 0;
    int     count = 0;

public:
    Fs_slot_item_data() = default;

    /**
      * \brief 기본 생성자 아이템 명칭 / 이미지 인덱스 / 개수 (UI매니저)
      * \param _name 아이템 명칭 \param _image_index UI 이미지 인덱스 \param _count 개수
     */
    Fs_slot_item_data(FString _name, int _image_index, int _count = 1) : name(_name), image_index(_image_index), count(_count) { }
};

UCLASS()
class UI_PUBG_API UItem_Slot_UI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
    /** \brief 현재 쓰여지는 변수 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UHorizontalBox* Main_horizontal_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox*       Background_size_box;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*        Item_border;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*         Item_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*     Name_txt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*     Count_txt;

    FDele_check_for_slot       dele_check_for_slot;
    FDele_set_weapon_slot_null dele_set_weapon_slot_null;
    Fs_slot_item_data          item_data;

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
      * \param _pos 슬롯 UI 위치 
     */
    void Set_as_cursor(FVector2D _pos);
};