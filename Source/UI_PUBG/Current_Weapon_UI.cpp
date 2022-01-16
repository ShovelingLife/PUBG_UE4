#include "Current_Weapon_UI.h"
#include "UI_manager.h"
#include "Characters/Custom_player.h"
#include "Player_weapons/Weapon_manager.h"
#include "Player_weapons/Core_weapon.h"
#include "Player_weapons/Core_melee_weapon.h"
#include "Player_weapons/Core_throwable_weapon.h"
#include "PUBG_UE4/Global.h"
#include "Kismet/GameplayStatics.h"

void UCurrent_Weapon_UI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCurrent_Weapon_UI::NativeTick(const FGeometry& _my_geometry, float _delta_time)
{
    Super::NativeTick(_my_geometry, _delta_time);

    if (!mp_weapon_manager)
        mp_weapon_manager = Cast<ACustom_player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->Get_weapon_manager();

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    Init_img_arr();
    Update_icon_visibility();
    Update_icon_color();
    Set_icon_UI();
}

void UCurrent_Weapon_UI::Init_img_arr()
{
    p_arr_img.Add(First_weapon_img);
    p_arr_img.Add(Second_weapon_img);
    p_arr_img.Add(Third_weapon_img);
    p_arr_img.Add(Fourth_weapon_img);
    p_arr_img.Add(Fifth_weapon_img);
}

void UCurrent_Weapon_UI::Update_icon_visibility()
{
    if (!mp_weapon_manager)
        return;
    
    // 첫번째 무기
    if (mp_weapon_manager->p_first_gun)
        First_weapon_img->SetVisibility(ESlateVisibility::Visible);

    else
        First_weapon_img->SetVisibility(ESlateVisibility::Hidden);

    // 두번째 무기
    if (mp_weapon_manager->p_second_gun)
        Second_weapon_img->SetVisibility(ESlateVisibility::Visible);

    else
        Second_weapon_img->SetVisibility(ESlateVisibility::Hidden);
    
    // 세번째 무기
    if (mp_weapon_manager->p_pistol)
        Third_weapon_img->SetVisibility(ESlateVisibility::Visible);

    else
        Third_weapon_img->SetVisibility(ESlateVisibility::Hidden);

    // 네번째 무기
    if (mp_weapon_manager->p_melee)
        Fourth_weapon_img->SetVisibility(ESlateVisibility::Visible);

    else
        Fourth_weapon_img->SetVisibility(ESlateVisibility::Hidden);

    // 다섯번째 무기
    if (mp_weapon_manager->p_throwable)
        Fifth_weapon_img->SetVisibility(ESlateVisibility::Visible);

    else
        Fifth_weapon_img->SetVisibility(ESlateVisibility::Hidden);
}

void UCurrent_Weapon_UI::Update_icon_color()
{
    if (!mp_weapon_manager)
        return;

    // 현재 선택된 무기
    e_current_weapon_type current_selected_weapon = mp_weapon_manager->current_weapon_type;

    // 선택 해제 및 탄알이 비어있을 시
    for (int i = 0; i < 3; i++)
    {
        if (mp_weapon_manager->Get_weapon_max_bullet_count((e_current_weapon_type)i) == 0)
            p_arr_img[i]->SetColorAndOpacity(mk_unselected_no_ammo_color);

        else
            p_arr_img[i]->SetColorAndOpacity(mk_unselected_color);
    }
    // 선택하고 있을 시
    if(current_selected_weapon != e_current_weapon_type::NONE)
    {
        // 선택 및 탄알이 비어있을 시
        if (mp_weapon_manager->Get_weapon_max_bullet_count((e_current_weapon_type)current_selected_weapon) == 0)
            p_arr_img[(int)current_selected_weapon]->SetColorAndOpacity(mk_selected_no_ammo_color);

        else
            p_arr_img[(int)current_selected_weapon]->SetColorAndOpacity(mk_selected_color);
    }
}

void UCurrent_Weapon_UI::Set_icon_UI()
{
    if (!mp_weapon_manager)
        return;
    
    // 첫번째 무기
    if (mp_weapon_manager->is_first_equipped)
        First_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_first_gun->weapon_type]);

    // 두번째 무기
    if (mp_weapon_manager->is_second_equipped)
        Second_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_second_gun->weapon_type]);

    // 세번째 무기
    if (mp_weapon_manager->is_third_equipped)
        Third_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_pistol->weapon_type]);
    
    // 네번쨰 무기
    if (mp_weapon_manager->is_fourth_equipped)
        Fourth_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_melee->weapon_type]);
    
    // 다섯번째 무기
    if (mp_weapon_manager->is_fifth_equipped)
        Fifth_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_throwable->weapon_type]);
}