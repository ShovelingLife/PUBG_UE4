#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotUI.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

UCLASS()
class UI_PUBG_API UCharacterSlotUI : public UUserWidget
{
    GENERATED_BODY()

public:
    // 장비 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* HeadSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* BackbagSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* BeltSlotImg;

    // 가방 여유 게이지바
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* BackbagFreeSpaceBar;

    // 방탄복 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     BodyarmorSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     ArmorIconImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* ArmorDurabilityText;

    // 스킨 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* HatSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* MaskSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CoatSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* ShirtSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* PantsSlotImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* ShoesSlotImg;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};