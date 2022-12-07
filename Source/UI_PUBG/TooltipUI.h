#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotItemData.h"
#include "TooltipUI.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class UI_PUBG_API UTooltipUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UImage*	   ItemImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* NameTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* CategoryTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* DescriptionTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* CapacityTxt;

protected:
	virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;

public:
	void SetData(FsSlotItemData SlotItemData);
};