#include "CurrentWeaponUI.h"
#include "GameInstanceSubsystemUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/WeaponEnum.h"
#include "Player_weapons/WeaponManager.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UCurrentWeaponUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitArrImg();
    pGameInstanceSubSystemUI = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>();
}

void UCurrentWeaponUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);
    UpdateIconVisibility();
    UpdateIconColor();
    SetIconUI();
}

void UCurrentWeaponUI::InitArrImg()
{
    pArrImg.Add(FirstWeaponImg);
    pArrImg.Add(SecondWeaponImg);
    pArrImg.Add(ThirdWeaponImg);
    pArrImg.Add(FourthWeaponImg);
    pArrImg.Add(FifthWeaponImg);
}

void UCurrentWeaponUI::UpdateIconVisibility()
{
    if (!pGameInstanceSubSystemUI)
        return;

    if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
    {
        FirstWeaponImg->SetVisibility((p_weaponManager->pFirstGun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        SecondWeaponImg->SetVisibility(p_weaponManager->pSecondGun ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        ThirdWeaponImg->SetVisibility((p_weaponManager->pPistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        FourthWeaponImg->SetVisibility((p_weaponManager->pMelee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        FifthWeaponImg->SetVisibility((p_weaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UCurrentWeaponUI::UpdateIconColor()
{
    if (!pGameInstanceSubSystemUI)
        return;

    auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    if (!p_weaponManager)
        return;

    // 현재 선택된 무기
    ECurrentWeaponType currentSelectedWeapon = p_weaponManager->CurrentWeaponType;
    
    // 선택 해제 및 탄알이 비어있을 시
    for (int i = 0; i < 3; i++)
    {
        if (p_weaponManager->GetMaxBulletCount((ECurrentWeaponType)i) == 0)
            pArrImg[i]->SetColorAndOpacity(mkUnselectedNoAmmoColor);

        else
            pArrImg[i]->SetColorAndOpacity(mkUnselectedColor);
    }
    pArrImg[3]->SetColorAndOpacity(mkUnselectedColor);
    pArrImg[4]->SetColorAndOpacity(mkUnselectedColor); 

    // 선택하고 있을 시
    if (currentSelectedWeapon != ECurrentWeaponType::NONE)
    {
        int index = (int)currentSelectedWeapon - 1;

        // 선택 및 탄알이 비어있을 시
        if (p_weaponManager->GetMaxBulletCount(currentSelectedWeapon) == 0)
            pArrImg[index]->SetColorAndOpacity(mkSelectedNoAmmoColor);

        else
            pArrImg[index]->SetColorAndOpacity(mkSelectedColor);
    }
}

void UCurrentWeaponUI::SetIconUI()
{
    if (!pGameInstanceSubSystemUI)
        return;

    auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager();

    if (!p_weaponManager)
        return;

    if (auto p_firstGun = p_weaponManager->pFirstGun)
    {
        if (FirstWeaponImg)
            FirstWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_firstGun->WeaponType));
    }
    if (auto p_secondGun = p_weaponManager->pSecondGun)
    {
        if (SecondWeaponImg)
            SecondWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_secondGun->WeaponType));
    }
    if (auto p_pistol = p_weaponManager->pPistol)
    {
        if (ThirdWeaponImg)
            ThirdWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_pistol->WeaponType));
    }
    //// 네번쨰 무기
    //if (mp_weapon_manager->p_melee)
    //{
    //    index = ((int)e_weapon_type::MAX) + ((int)mp_weapon_manager->p_melee->weapon_type);
    //    Fourth_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[index]);
    //}
    //// 다섯번째 무기
    //if (mp_weapon_manager->p_throwable)
    //{
    //    index = ((int)e_weapon_type::MAX) + ((int)mp_weapon_manager->p_throwable->weapon_type);
    //    Fifth_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[index]);
    //}
}