#include "CurrentWeaponUI.h"
#include "GameInstanceSubsystemUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/CoreMeleeWeapon.h"
#include "Player_weapons/CoreThrowableWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UCurrentWeaponUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitArrImg();    
}

void UCurrentWeaponUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

    if (!pWeaponManager)
    {
        if (auto p_subGameInst = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UGameInstanceSubsystemUI>())
            pWeaponManager = p_subGameInst->GetWeaponManager();
    }
    else
    {
        UpdateIconVisibility();
        UpdateIconColor();
        SetIconUI();
    }
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
    using enum ESlateVisibility;

    FirstWeaponImg->SetVisibility((pWeaponManager->pFirstGun)  ? Visible : Hidden);
    SecondWeaponImg->SetVisibility(pWeaponManager->pSecondGun  ? Visible : Hidden);
    ThirdWeaponImg->SetVisibility((pWeaponManager->pPistol)    ? Visible : Hidden);
    FourthWeaponImg->SetVisibility((pWeaponManager->pMelee)    ? Visible : Hidden);
    FifthWeaponImg->SetVisibility((pWeaponManager->pThrowable) ? Visible : Hidden);
}

void UCurrentWeaponUI::UpdateIconColor()
{
    // 현재 선택된 무기
    EWeaponType currentSelectedWeapon = pWeaponManager->CurrentType;

    if (currentSelectedWeapon == EWeaponType::NONE)
        return;

    // 선택 해제 및 탄알이 비어있을 시
    for (int i = 0; i < 3; i++)
    {
        if (auto p_weapon = Cast<ACoreWeapon>(pWeaponManager->GetWeaponByIndex((EWeaponType)(i + 1))))
            pArrImg[i]->SetColorAndOpacity((p_weapon->WeaponData.BulletCount == 0) ? mkUnselectedNoAmmoColor : mkUnselectedColor);
    }
    pArrImg[3]->SetColorAndOpacity(mkUnselectedColor);
    pArrImg[4]->SetColorAndOpacity(mkUnselectedColor);

    // 선택하고 있을 시
    int index = (int)currentSelectedWeapon - 1;
    int currentBulletCount = 0;

    if (auto p_weapon = pWeaponManager->GetWeaponByIndex(currentSelectedWeapon))
    {
        if (p_weapon->IsA<ACoreWeapon>())
        {
            auto p_gun = Cast<ACoreWeapon>(p_weapon);
            currentBulletCount = p_gun->WeaponData.BulletCount;
        }
        else if (p_weapon->IsA<ACoreMeleeWeapon>() ||
                 p_weapon->IsA<ACoreThrowableWeapon>())
                 currentBulletCount = 1;
    }
    pArrImg[index]->SetColorAndOpacity((currentBulletCount == 0) ? mkSelectedNoAmmoColor : mkSelectedColor);
}

void UCurrentWeaponUI::SetIconUI()
{
    if (auto p_firstGun = pWeaponManager->pFirstGun)
        FirstWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_firstGun->WeaponType));

    if (auto p_secondGun = pWeaponManager->pSecondGun)
        SecondWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_secondGun->WeaponType));

    if (auto p_pistol = pWeaponManager->pPistol)
        ThirdWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_pistol->WeaponType));

    // 네번쨰 무기
    //if (auto p_melee = p_weaponManager->pMelee)
    //    FourthWeaponImg->SetBrushFromMaterial(AUI_manager::GetMaterial((int)p_melee->Wea);

    // 다섯번째 무기
    if (auto p_throwable = pWeaponManager->pThrowable)
        FifthWeaponImg->SetBrushFromTexture(AUI_manager::GetTexture2D((int)p_throwable->WeaponType, "Explosive"));
}