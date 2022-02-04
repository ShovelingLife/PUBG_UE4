#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character_Slot_UI.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

UCLASS()
class UI_PUBG_API UCharacter_Slot_UI : public UUserWidget
{
    GENERATED_BODY()

public:
    // 장비 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Head_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Backbag_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Belt_slot_img;

    // 가방 여유 게이지바
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* Backbag_free_space_bar;

    // 방탄복 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     Bodyarmor_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     Armor_icon_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Armor_durability_text;

    // 스킨 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Hat_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Mask_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Coat_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Shirts_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Pants_slot_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* Shoes_slot_img;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};