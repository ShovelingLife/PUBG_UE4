#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotUI.generated.h"

class UEquipmentSlotUI;
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
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UImage* HighlightImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* CharacterImg;

// ------- 장비 관련 4개 -------
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* HelmetSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* BodyArmorSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* BackpackSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* BeltSlotUI;

// ------- 방탄복 관련 -------
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* ArmorIconImg;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* ArmorDurabilityText;

// ------- 스킨 관련 7개 -------
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* HatSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* MaskSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* CoatSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* ShirtSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* GlovesSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* PantsSlotUI;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UEquipmentSlotUI* ShoesSlotUI;

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
    void UpdateHighlightImg();

    void RotateDummyCharacterUI();
};