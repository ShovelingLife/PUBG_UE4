#include "PlayerStateUI.h"
#include "PlayerEffectUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/CoreWeapon.h"
#include "Player_weapons/WeaponManager.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPlayerStateUI::NativeConstruct()
{
    Super::NativeConstruct();
    p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UPlayerStateUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

    if (!mpUI_manager)
        mpUI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    if (p_player)
    {
        UpdateAimUI();
        UpdateBulletCountUI();
        UpdateShootMode();
        UpdateHealthBarUI(DeltaTime);
        UpdateOxygenBarUI(DeltaTime);
    }
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
    auto p_weapon        = p_weaponManager->GetWeaponByIndex(p_weaponManager->CurrentWeaponType);

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
    auto p_weapon        = p_weaponManager->GetWeaponByIndex(p_weaponManager->CurrentWeaponType);

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

void UPlayerStateUI::UpdateHealthBarUI(float DeltaTime)
{
    UCustomGameInstance* p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
    auto currentState = p_player->CurrentState;

    if (!p_customGameInst)
        return;

    if (currentState == EPlayerState::DEAD)
    {
        if (p_customGameInst)
            p_customGameInst->DeleKillUI_Anim.ExecuteIfBound();
        
        auto controller = GetWorld()->GetFirstPlayerController();
        controller->DisableInput(controller);
        GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("플레이어가 사망한 상태입니다.")));
        return;
    }
    float currentHealth = p_player->CurrentHealth;
    float maxHealth = p_player->kMaxHealth;

    // 현재 HP 설정
    if (currentHealth > 0.f)
    {
        // 화상 타이머 설정
        if (p_player->CurrentOtherState == EPlayerOtherState::BURNED)
        {
            mCurrentTime += DeltaTime;

            if (mCurrentTime >= 0.5f)
            {
                p_player->CurrentHealth -= 2.5f;
                mCurrentTime = 0.f;
            }
            p_customGameInst->DeleRunEffectAnim.ExecuteIfBound(0.f, 3.f, EPlayerStateAnimType::BURNED);
        }
        HP_bar->SetPercent(currentHealth / maxHealth);
    }
    // 추가 HP 설정
    else
    {
        HP_bar->SetVisibility(ESlateVisibility::Hidden);
        
        // 부상 타이머 설정
        mCurrentTime += DeltaTime;

        if (mCurrentTime >= 0.5f)
        {
            p_player->CurrentInjuredHealth -= 2.5f;
            mCurrentTime = 0.f;
        }
        p_customGameInst->DeleRunEffectAnim.ExecuteIfBound(0.f, 3.f, EPlayerStateAnimType::INJURED);
        Injured_HP_bar->SetPercent(p_player->CurrentInjuredHealth / maxHealth);
    }
}

void UPlayerStateUI::UpdateOxygenBarUI(float DeltaTime)
{
    // 현재 뛰고있음
    if (p_player->CurrentState == EPlayerState::SPRINT ||
        p_player->CurrentState == EPlayerState::SPRINT_JUMP)
    {
        if (p_player->CurrentOxygen > 0.f)
            p_player->CurrentOxygen -= 0.001f;
    }
    else
    {
        if (p_player->CurrentOxygen < 1.f)
            p_player->CurrentOxygen += (DeltaTime * 0.03);
    }
    OxygenBar->SetPercent(p_player->CurrentOxygen);
}