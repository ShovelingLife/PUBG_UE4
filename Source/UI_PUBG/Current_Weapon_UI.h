// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Current_Weapon_UI.generated.h"

/**
 * 
 */
UCLASS()
class UI_PUBG_API UCurrent_Weapon_UI : public UUserWidget
{
	GENERATED_BODY()
	


public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* First_weapon_img;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Second_weapon_img;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Third_weapon_img;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Fourth_weapon_img;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Fifth_weapon_img;




protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};
