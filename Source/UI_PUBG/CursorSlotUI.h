#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CursorSlotUI.generated.h"

class UImage;
class USizeBox;

UCLASS()
class UI_PUBG_API UCursorSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) USizeBox* SB_Background;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* ItemImg; 
    FVector2D ClickPos;

public:
	virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;
};