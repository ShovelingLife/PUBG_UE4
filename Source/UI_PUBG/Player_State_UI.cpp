#include "Player_State_UI.h"
#include "UI_manager.h"
#include "Characters/Custom_player.h"
#include "Kismet/GameplayStatics.h"
#include "Player_weapons/Core_weapon.h"
#include "Player_weapons/Weapon_manager.h"

void UPlayer_State_UI::NativeConstruct()
{
    Super::NativeConstruct();
    p_player = Cast<ACustom_player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UPlayer_State_UI::NativeTick(const FGeometry& _my_geometry, float _delta_time)
{
    Super::NativeTick(_my_geometry, _delta_time);

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    Update_aim_UI();
    Update_bullet_count_UI();
    Update_shoot_mode();
    Update_oxygen_bar_UI(_delta_time);
}

void UPlayer_State_UI::Update_aim_UI()
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

void UPlayer_State_UI::Update_bullet_count_UI()
{
    auto p_weapon_manager = p_player->Get_weapon_manager();
    auto p_weapon         = p_weapon_manager->Get_weapon_type(p_weapon_manager->current_weapon_type);

    // 현재 및 최대 총알 개수 설정
    if (p_weapon)
    {
        Magazine_current_text->SetText(FText::FromString(FString::FromInt(p_weapon->weapon_data.current_bullet_count)));
        Magazine_total_text->SetText(FText::FromString(FString::FromInt(p_weapon->weapon_data.max_bullet_count)));
    }
}

void UPlayer_State_UI::Update_shoot_mode()
{
    auto p_weapon_manager = p_player->Get_weapon_manager();
    auto p_weapon         = p_weapon_manager->Get_weapon_type(p_weapon_manager->current_weapon_type);

    if (p_weapon &&
        mp_UI_manager)
        BoltAction_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_player_ui_tex[(int)p_weapon->gun_shoot_type]));
}

void UPlayer_State_UI::Update_oxygen_bar_UI(float _delta_time)
{
    // 현재 뛰고있음
    if      (p_player->is_sprinting &&
             p_player->current_oxygen > 0.f)
             p_player->current_oxygen -= 0.001f;

    else if (!p_player->is_sprinting &&
              p_player->current_oxygen < 1.f)
              p_player->current_oxygen += (_delta_time * 0.03);

    Oxygen_bar->SetPercent(p_player->current_oxygen);
}