// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "My_Item_Slot_UI.generated.h"

/**
 * 
 */
UCLASS()
class UI_PUBG_API UMy_Item_Slot_UI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* My_item_img;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Around_item_name_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Around_item_amount_txt;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
	
};
