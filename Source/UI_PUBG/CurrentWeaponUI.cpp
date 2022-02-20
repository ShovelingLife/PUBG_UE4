#include "CurrentWeaponUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Player_weapons/WeaponEnum.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UCurrentWeaponUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitArrImg();
}

void UCurrentWeaponUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);

    if (!mpWeaponManager)
        mpWeaponManager = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetWeaponManager();

    if (!mpUI_Manager)
        mpUI_Manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

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
    if (!mpWeaponManager)
        return;
    
    FirstWeaponImg->SetVisibility((mpWeaponManager->pFirstGun)  ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    SecondWeaponImg->SetVisibility(mpWeaponManager->pSecondGun  ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    ThirdWeaponImg->SetVisibility((mpWeaponManager->pPistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FourthWeaponImg->SetVisibility((mpWeaponManager->pMelee)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    FifthWeaponImg->SetVisibility((mpWeaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCurrentWeaponUI::UpdateIconColor()
{
    if (!mpWeaponManager)
        return;

    // 현재 선택된 무기
    ECurrentWeaponType currentSelectedWeapon = mpWeaponManager->CurrentWeaponType;
    
    // 선택 해제 및 탄알이 비어있을 시
    for (int i = 0; i < 3; i++)
    {
        if (mpWeaponManager->GetMaxBulletCount((ECurrentWeaponType)i) == 0)
            pArrImg[i]->SetColorAndOpacity(mkUnselectedNoAmmoColor);

        else
            pArrImg[i]->SetColorAndOpacity(mkUnselectedColor);
    }
    pArrImg[3]->SetColorAndOpacity(mkUnselectedColor);
    pArrImg[4]->SetColorAndOpacity(mkUnselectedColor); 

    // 선택하고 있을 시
    if(currentSelectedWeapon != ECurrentWeaponType::NONE)
    {
        int index = (int)currentSelectedWeapon - 1;

        // 선택 및 탄알이 비어있을 시
        if (mpWeaponManager->GetMaxBulletCount(currentSelectedWeapon) == 0)
            pArrImg[index]->SetColorAndOpacity(mkSelectedNoAmmoColor);

        else
            pArrImg[index]->SetColorAndOpacity(mkSelectedColor);
    }
}

void UCurrentWeaponUI::SetIconUI()
{
    if (!mpWeaponManager &&
        !mpUI_Manager)
        return;
    
    if (mpWeaponManager->pFirstGun)
        FirstWeaponImg->SetBrushFromMaterial(mpUI_Manager->MapMainWeaponMat[(int)mpWeaponManager->pFirstGun->WeaponType]);

    if (mpWeaponManager->pSecondGun)
        SecondWeaponImg->SetBrushFromMaterial(mpUI_Manager->MapMainWeaponMat[(int)mpWeaponManager->pSecondGun->WeaponType]);

    if (mpWeaponManager->pPistol)
        ThirdWeaponImg->SetBrushFromMaterial(mpUI_Manager->MapMainWeaponMat[(int)mpWeaponManager->pPistol->WeaponType]);
    
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