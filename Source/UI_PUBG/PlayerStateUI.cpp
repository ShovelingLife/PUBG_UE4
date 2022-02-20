#include "PlayerStateUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayerStateUI::NativeConstruct()
{
    Super::NativeConstruct();
    p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UPlayerStateUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);

    if (!mpUI_manager)
        mpUI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    UpdateAimUI();
    UpdateBulletCountUI();
    UpdateShootMode();
    UpdateOxygenBarUI(_DeltaTime);
}

void UPlayerStateUI::UpdateAimUI()
{
    //// ?먯엫
    //if (is_aiming)
    //{
    //    FWidgetTransform reticle_img_trans;
    //    float mouse_posX = 0.f, mouse_posY = 0.f;
    //    mp_user_ui->Reticle_img->SetVisibility(ESlateVisibility::Visible);
    //    reticle_img_trans.Translation.Set(mouse_posX, mouse_posY);
    //    UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouse_posX, mouse_posY);
    //    mp_user_ui->Reticle_img->SetRenderTransform(reticle_img_trans);
    //}
    //else
    //    mp_user_ui->Reticle_img->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerStateUI::UpdateBulletCountUI()
{
    auto p_weaponManager = p_player->GetWeaponManager();
    auto p_weapon         = p_weaponManager->GetWeaponByIndex(p_weaponManager->CurrentWeaponType);

    if (!p_weapon)
        return;

    // 현재 및 최대 총알 개수 설정
    if (p_weapon->IsA<ACoreWeapon>())
    {
        auto p_gun = Cast<ACoreWeapon>(p_weapon);
        MagazineCurrentTxt->SetText(FText::FromString(FString::FromInt(p_gun->WeaponData.CurrentBulletCount)));
        MagazineTotalTxt->SetText(FText::FromString(FString::FromInt(p_gun->WeaponData.MaxBulletCount)));
    }
}

void UPlayerStateUI::UpdateShootMode()
{
    auto p_weaponManager = p_player->GetWeaponManager();
    auto p_weapon         = p_weaponManager->GetWeaponByIndex(p_weaponManager->CurrentWeaponType);

    if (!p_weapon)
        return;

    if (mpUI_manager &&
        p_weapon->IsA<ACoreWeapon>())
    {
        auto p_gun = Cast<ACoreWeapon>(p_weapon);
        BoltActionImg->SetVisibility(ESlateVisibility::Visible);
        BoltActionImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapPlayerTex[(int)p_gun->ShootType]));
    }
    else
        BoltActionImg->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerStateUI::UpdateOxygenBarUI(float _DeltaTime)
{
    // 현재 뛰고있음
    if      (p_player->bSprinting &&
             p_player->CurrentOxygen > 0.f)
             p_player->CurrentOxygen -= 0.001f;

    else if (!p_player->bSprinting &&
              p_player->CurrentOxygen < 1.f)
              p_player->CurrentOxygen += (_DeltaTime * 0.03);

    OxygenBar->SetPercent(p_player->CurrentOxygen);
}