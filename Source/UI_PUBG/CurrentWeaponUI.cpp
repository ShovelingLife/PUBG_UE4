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

void UCurrentWeaponUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
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
    if (pGameInstanceSubSystemUI)
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
        {
            FirstWeaponImg->SetVisibility((p_weaponManager->pFirstGun)  ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
            SecondWeaponImg->SetVisibility(p_weaponManager->pSecondGun  ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
            ThirdWeaponImg->SetVisibility((p_weaponManager->pPistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
            FourthWeaponImg->SetVisibility((p_weaponManager->pMelee)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
            FifthWeaponImg->SetVisibility((p_weaponManager->pThrowable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}

void UCurrentWeaponUI::UpdateIconColor()
{
    if (pGameInstanceSubSystemUI)
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
        {
            // 현재 선택된 무기
            ECurrentWeaponType currentSelectedWeapon = p_weaponManager->CurrentWeaponType;

            if (currentSelectedWeapon == ECurrentWeaponType::NONE)
                return;

            // 선택 해제 및 탄알이 비어있을 시
            for (int i = 0; i < 3; i++)
            {
                if (auto p_weapon = Cast<ACoreWeapon>(p_weaponManager->GetWeaponByIndex((ECurrentWeaponType)(i + 1))))
                    pArrImg[i]->SetColorAndOpacity((p_weapon->WeaponData.CurrentBulletCount == 0) ? mkUnselectedNoAmmoColor : mkUnselectedColor);       
            }
            pArrImg[3]->SetColorAndOpacity(mkUnselectedColor);
            pArrImg[4]->SetColorAndOpacity(mkUnselectedColor);

            // 선택하고 있을 시
            int index = (int)currentSelectedWeapon - 1;
            int currentBulletCount = 0;

            if (auto p_weapon = p_weaponManager->GetWeaponByIndex(currentSelectedWeapon))
            {
                if (p_weapon->IsA<ACoreWeapon>())
                {
                    auto p_gun = Cast<ACoreWeapon>(p_weapon);
                    currentBulletCount = p_gun->WeaponData.CurrentBulletCount;
                }
                else if (p_weapon->IsA<ACoreMeleeWeapon>() ||
                         p_weapon->IsA<ACoreThrowableWeapon>())
                         currentBulletCount = 1;
            }
            pArrImg[index]->SetColorAndOpacity((currentBulletCount == 0) ? mkSelectedNoAmmoColor : mkSelectedColor);
        }
    }
}

void UCurrentWeaponUI::SetIconUI()
{
    if (pGameInstanceSubSystemUI)
    {
        if (auto p_weaponManager = pGameInstanceSubSystemUI->GetWeaponManager())
        {
            if (auto p_firstGun = p_weaponManager->pFirstGun)
                FirstWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_firstGun->WeaponType));

            if (auto p_secondGun = p_weaponManager->pSecondGun)
                SecondWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_secondGun->WeaponType));

            if (auto p_pistol = p_weaponManager->pPistol)
                ThirdWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_pistol->WeaponType));

            // 네번쨰 무기
            //if (auto p_melee = p_weaponManager->pMelee)
            //    FourthWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_melee->Wea);

            // 다섯번째 무기
            if (auto p_throwable = p_weaponManager->pThrowable)
                FifthWeaponImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_throwable->CurrentWeaponType));
        }
    }
}