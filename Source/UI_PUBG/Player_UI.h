// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player_UI.generated.h"

/**
 * 
 */
UCLASS()
class UI_PUBG_API UPlayer_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    // 체력 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Health_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Energy_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Oxygen_bar;

    // 총알 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Ammo_txt;

    // 총알 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Shoot_type_txt;

    // 무기 관련
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* First_weapon_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Second_weapon_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Weapon_select_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Reticle_img;


protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};