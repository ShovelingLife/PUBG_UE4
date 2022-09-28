#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotUI.generated.h"

class UItemSlotUI;
class UBorder;
class UImage;
class UProgressBar;
class UTextBlock;

UCLASS()
class UI_PUBG_API UCharacterSlotUI : public UUserWidget
{
    GENERATED_BODY()

private:
    FVector2D mCharacterUIClickPos = FVector2D::ZeroVector;
    /*UPROPERTY(EditAnywhere, category = var) float xval1;
    UPROPERTY(EditAnywhere, category = var) float xval2;*/

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      HighlightImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      CharacterImg;

    // 장비 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* HeadSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     HeadSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      HeadSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* BodyArmorSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     BodyArmorSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      BodyArmorSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* BackpackSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     BackpackSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      BackpackSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* BeltSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     BeltSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      BeltSlotImg;

    // 방탄복 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      ArmorIconImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*  ArmorDurabilityText;

    // 스킨 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* HatSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     HatSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      HatSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* MaskSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     MaskSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      MaskSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* CoatSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     CoatSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      CoatSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* ShirtSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     ShirtSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      ShirtSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* GlovesSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     GlovesSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      GlovesSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* PantsSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     PantsSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      PantsSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UItemSlotUI* ShoesSlotUI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder*     ShoesSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*      ShoesSlotImg;
    bool bHoveringCharacterSlot;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry&, float) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    void InitSlotUI();

    void UpdateHighlightImg();


};