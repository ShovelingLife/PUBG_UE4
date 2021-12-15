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
    // ü�� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Health_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Energy_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Oxygen_bar;

    // �Ѿ� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Ammo_txt;

    // �Ѿ� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Shoot_type_txt;

    // ���� ����
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