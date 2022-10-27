#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotUI.generated.h"

class UBorder;
class UImage;

UCLASS()
class UI_PUBG_API UEquipmentSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* ItemBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* ItemImg;


};