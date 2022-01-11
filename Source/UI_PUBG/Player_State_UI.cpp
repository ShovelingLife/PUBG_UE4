#include "Player_State_UI.h"
#include "Characters/Custom_player.h"
#include "Kismet/GameplayStatics.h"

void UPlayer_State_UI::NativeConstruct()
{
    Super::NativeConstruct();
    p_custom_player = Cast<ACustom_player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UPlayer_State_UI::NativeTick(const FGeometry& _my_geometry, float _delta_time)
{
    Super::NativeTick(_my_geometry, _delta_time);
    Update_oxygen_bar_UI(_delta_time);
    Update_weapon_slot_UI();
    Update_aim_UI();
    Update_bullet_count_UI();
}

void UPlayer_State_UI::Update_weapon_slot_UI()
{
    //FSlateBrush      slate_brush;
    //FWidgetTransform select_img_trans;
    //slate_brush.SetImageSize(FVector2D(350.f, 75.f));

    //// 착용한 무기가 없을 시
    //if (!m_first_weapon)
    //    Weapon_select_img->SetVisibility(ESlateVisibility::Hidden);

    //else
    //{
    //    mp_user_ui->Weapon_select_img->SetVisibility(ESlateVisibility::Visible);
    //    slate_brush.SetResourceObject(m_first_weapon->p_render_target_ui_mesh);
    //    mp_user_ui->First_weapon_img->SetBrush(slate_brush);

    //    // 첫번째 무기 착용 했을 시
    //    if (m_first_weapon->is_equipped)
    //    {
    //        select_img_trans.Translation.Set(0.f, -150.f);
    //        mp_user_ui->Weapon_select_img->SetRenderTransform(select_img_trans);
    //    }
    //}
    //if (m_second_weapon)
    //{
    //    slate_brush.SetResourceObject(m_second_weapon->p_render_target_ui_mesh);
    //    mp_user_ui->Second_weapon_img->SetBrush(slate_brush);

    //    // 두번째 무기 착용 했을 시
    //    if (m_second_weapon->is_equipped)
    //    {
    //        select_img_trans.Translation.Set(0.f, -75.f);
    //        mp_user_ui->Weapon_select_img->SetRenderTransform(select_img_trans);
    //    }
    //}
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
    /*bool is_first_equipped = false;
    bool is_second_equipped = false;

    Verify_equipped_weapon(is_first_equipped, is_second_equipped);

    if (is_first_equipped)
    {
        FString str = FString::FromInt(m_first_weapon->weapon_data.current_bullet_count) + "/" + FString::FromInt(m_first_weapon->weapon_data.current_ammunition_count);
        mp_user_ui->Ammo_txt->SetText(FText::FromString(str));
    }
    else
    {
        if (is_second_equipped)
        {
            FString str = FString::FromInt(m_second_weapon->weapon_data.current_bullet_count) + "/" + FString::FromInt(m_second_weapon->weapon_data.current_ammunition_count);
            mp_user_ui->Ammo_txt->SetText(FText::FromString(str));
        }
        else
            mp_user_ui->Ammo_txt->SetText(FText::FromString(""));
    }*/
}

void UPlayer_State_UI::Update_oxygen_bar_UI(float _delta_time)
{
    // 현재 뛰고있음
    if      (p_custom_player->is_sprinting &&
             p_custom_player->current_oxygen > 0.f)
             p_custom_player->current_oxygen -= 0.001f;

    else if (!p_custom_player->is_sprinting &&
              p_custom_player->current_oxygen < 1.f)
              p_custom_player->current_oxygen += (_delta_time * 0.03);

    Oxygen_bar->SetPercent(p_custom_player->current_oxygen);
}

void UPlayer_State_UI::Change_shoot_mode()
{
    /*if (!m_is_changed_shoot_type)
    {
        m_is_changed_shoot_type = true;
        m_shoot_time = 0.25f;
        Shoot_type_txt->SetText(FText::FromString(FString::Printf(TEXT("모드:연사"), *FString(""))));
    }
    else
    {
        m_is_changed_shoot_type = false;
        m_shoot_time = 0.1f;
        Shoot_type_txt->SetText(FText::FromString(FString::Printf(TEXT("모드:점사"), *FString(""))));
    }*/
}