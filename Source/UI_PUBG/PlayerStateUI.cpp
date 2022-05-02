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
        if (auto p_weaponManager = p_player->GetWeaponManager())
        {
            if (auto p_weapon = p_weaponManager->GetWeaponByIndex(p_weaponManager->CurrentWeaponType))
            {
                UpdateBulletCountUI(p_weapon);
                UpdateShootMode(p_weapon);
            }
        }
        UpdateHealthBarUI(DeltaTime);
        UpdateOxygenBarUI(DeltaTime);
    }
}

void UPlayerStateUI::UpdateBulletCountUI(ABaseInteraction* pWeapon)
{
    // 현재 및 최대 총알 개수 설정
    if (auto p_gun = Cast<ACoreWeapon>(pWeapon))
    {
        auto weaponData = p_gun->WeaponData;
        MagazineCurrentTxt->SetText(FText::FromString(FString::FromInt(weaponData.CurrentBulletCount)));
        MagazineTotalTxt->SetText(FText::FromString(FString::FromInt(weaponData.MaxBulletCount)));
    }
}

void UPlayerStateUI::UpdateShootMode(ABaseInteraction* pWeapon)
{
    // 총기 격발 방식 변경 (단발/점사/연사)
    if (mpUI_manager)
    {
        if (auto p_gun = Cast<ACoreWeapon>(pWeapon))
        {
            int index = (int)p_gun->ShootType;
            BoltActionImg->SetVisibility(ESlateVisibility::Visible);
            BoltActionImg->SetBrushFromTexture(Cast<UTexture2D>(mpUI_manager->MapPlayerTex[index]));
        }
    }
}

void UPlayerStateUI::UpdateHealthBarUI(float DeltaTime)
{
    UCustomGameInstance* p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());

    if (!p_customGameInst)
        return;

    if (p_player->CurrentState == EPlayerState::DEAD)
    {
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