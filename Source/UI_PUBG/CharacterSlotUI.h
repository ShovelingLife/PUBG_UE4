﻿#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotUI.generated.h"

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

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* HighlightImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CharacterImg;

    // 장비 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* HeadSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  HeadSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* BodyArmorSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  BodyArmorSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* BackpackSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  BackpackSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* BeltSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  BeltSlotImg;

    // 가방 여유 게이지바
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* BackpackFreeSpaceBar;

    // 방탄복 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*     ArmorIconImg;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* ArmorDurabilityText;

    // 스킨 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* HatSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  HatSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* MaskSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  MaskSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* CoatSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  CoatSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* ShirtSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  ShirtSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* GlovesSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  GlovesSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* PantsSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  PantsSlotImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* ShoesSlotBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  ShoesSlotImg;
    bool bHoveringCharacterSlot;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;

    virtual FReply NativeOnMouseButtonUp(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;

    virtual FReply NativeOnMouseMove(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent, UDragDropOperation*& _OutOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

    virtual bool NativeOnDragOver(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

    virtual bool NativeOnDrop(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation) override;

private:
    void UpdateHighlightImg();
};