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
	
private:
	// 슬롯 이미지 사이즈 관련
	const FVector mk_gun_slot_size	  {	200.f, 60.f, 0.f};
	const FVector mk_pistol_slot_size {  85.f, 60.f, 0.f};
	const FVector mk_grenade_slot_size{  25.f, 30.f, 0.f };

	// 슬롯 이미지 위치 관련
    const FVector mk_gun_slot_pos	 { 200.f, 60.f, 0.f };
    const FVector mk_pistol_slot_pos {  85.f, 60.f, 0.f };
    const FVector mk_grenade_slot_pos{  25.f, 30.f, 0.f };

	// 슬롯 이미지 색상 관련
	const FVector mk_selected_color			 { 0.25f, 0.25f, 0.25f};
	const FVector mk_unselected_color		 {   1.f,   1.f,   1.f};
    const FVector mk_selected_no_ammo_color  {   1.f,   0.f,   0.f};
    const FVector mk_unselected_no_ammo_color{ 0.25f,   0.f,   0.f};

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

    // 메인 UI에 들어감
    UPROPERTY(VisibleAnywhere, Category = Weapon_UI)
        TMap<int, UMaterial*> map_weapon_ui_mat;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};