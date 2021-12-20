// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Weapon_Slot_UI.generated.h"

/**
 * 
 */
UCLASS()
class UI_PUBG_API UWeapon_Slot_UI : public UUserWidget
{
	GENERATED_BODY()
	

public:
    //1¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_1_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type_1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_1_txt;

    //2¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_2_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type_2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_2_txt;

    //3¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_3_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type_3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_3_txt;

    //4¹ø ½½·Ô (±ÙÁ¢¹«±â)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_4_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_4_txt;

    //5¹ø ½½·Ô (ÅõÃ´¹«±â)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_5_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_5_txt;


protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};
