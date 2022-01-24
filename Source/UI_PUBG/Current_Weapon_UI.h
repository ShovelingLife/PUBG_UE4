// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Current_Weapon_UI.generated.h"

class AWeapon_manager;

UCLASS()
class UI_PUBG_API UCurrent_Weapon_UI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    class AUI_manager* mp_UI_manager;

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
	const FLinearColor mk_selected_color		  {   1.f,   1.f,   1.f, 1.f };
	const FLinearColor mk_selected_no_ammo_color  {   1.f,   0.f,   0.f, 1.f };
	const FLinearColor mk_unselected_color		  { 0.25f, 0.25f, 0.25f, 1.f };
    const FLinearColor mk_unselected_no_ammo_color{ 0.25f,   0.f,   0.f, 1.f };

    // 현재 착용 중인 무기
    UPROPERTY(VisibleAnywhere, Category = Manager)
    AWeapon_manager* mp_weapon_manager = nullptr;

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

    // 현재 이미지 배열
	TArray<UImage*> p_arr_img;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;

private:
	void Init_arr_img();

	// UI 투명도 갱신
	void Update_icon_visibility();

	// UI 색상 갱신
	void Update_icon_color();

	// UI 등록
	void Set_icon_UI();
};