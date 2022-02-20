/**
 * \file Current_Weapon_UI.h
 *
 * \brief 현재 착용 중인 무기 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentWeaponUI.generated.h"

class AUI_manager;
class AWeaponManager;
class UImage;

UCLASS()
class UI_PUBG_API UCurrentWeaponUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /** \brief 현재 착용 중인 무기 */
    UPROPERTY(VisibleAnywhere, Category = Manager) AWeaponManager* mpWeaponManager = nullptr;

	/** \brief UI 매니저 */
    UPROPERTY() AUI_manager* mpUI_Manager;

	/**
	 * \brief 슬롯 이미지 사이즈 관련
	 */
	const FVector mkGunSlotSize	   {	200.f, 60.f, 0.f};
	const FVector mkPistolSlotSize {  85.f, 60.f, 0.f};
	const FVector mkGrenadeSlotSize{  25.f, 30.f, 0.f };

	/**
	 * \brief 슬롯 이미지 위치 관련
	 */
    const FVector mkGunSlotPos	  { 200.f, 60.f, 0.f };
    const FVector mkPistolSlotPos {  85.f, 60.f, 0.f };
    const FVector mkGrenadeSlotPos{  25.f, 30.f, 0.f };

	/**
	 * \brief 슬롯 이미지 색상 관련
	 */
	const FLinearColor mkSelectedColor		  {   1.f,   1.f,   1.f, 1.f };
	const FLinearColor mkSelectedNoAmmoColor  {   1.f,   0.f,   0.f, 1.f };
	const FLinearColor mkUnselectedColor	  { 0.25f, 0.25f, 0.25f, 1.f };
    const FLinearColor mkUnselectedNoAmmoColor{ 0.25f,   0.f,   0.f, 1.f };

public:
	/**
	 * \brief 현재 착용 중인 무기 아이콘
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* FirstWeaponImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* SecondWeaponImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* ThirdWeaponImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* FourthWeaponImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* FifthWeaponImg;

	/** \brief 현재 이미지 배열 */
	TArray<UImage*> pArrImg;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;

private:
	/**
	 * \brief 이미지 배열 초기화
	 */
	void InitArrImg();

	/**
	 * \brief UI 투명도 갱신
	 */
	void UpdateIconVisibility();

	/**
	 * \brief UI 색상 갱신
	 */
	void UpdateIconColor();

	/**
	 * \brief UI 등록
	 */
	void SetIconUI();
};