#include "Current_Weapon_UI.h"
#include "UI_manager.h"
#include "Characters/Custom_player.h"
#include "Player_weapons/Weapon_manager.h"
#include "Player_weapons/Weapon_enum.h"
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

    Init_arr_img();
    Update_icon_visibility();
    Update_icon_color();
    Set_icon_UI();
}

void UCurrent_Weapon_UI::Init_arr_img()
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
    
    First_weapon_img->SetVisibility((mp_weapon_manager->p_first_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_weapon_img->SetVisibility(mp_weapon_manager->p_second_gun ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Third_weapon_img->SetVisibility((mp_weapon_manager->p_pistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Fourth_weapon_img->SetVisibility((mp_weapon_manager->p_melee)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Fifth_weapon_img->SetVisibility((mp_weapon_manager->p_throwable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCurrent_Weapon_UI::Update_icon_color()
{
    if (!mp_weapon_manager)
        return;

    // ���� ���õ� ����
    e_current_weapon_type current_selected_weapon = mp_weapon_manager->current_weapon_type;

    // ���� ���� �� ź���� ������� ��
    for (int i = 0; i < 3; i++)
    {
        if (mp_weapon_manager->Get_weapon_max_bullet_count((e_current_weapon_type)i) == 0)
            p_arr_img[i]->SetColorAndOpacity(mk_unselected_no_ammo_color);

        else
            p_arr_img[i]->SetColorAndOpacity(mk_unselected_color);
    }
    p_arr_img[3]->SetColorAndOpacity(mk_unselected_color);
    p_arr_img[4]->SetColorAndOpacity(mk_unselected_color);

    // �����ϰ� ���� ��
    if(current_selected_weapon != e_current_weapon_type::NONE)
    {
        // ���� �� ź���� ������� ��
        if (mp_weapon_manager->Get_weapon_max_bullet_count((e_current_weapon_type)current_selected_weapon) == 0)
            p_arr_img[(int)current_selected_weapon]->SetColorAndOpacity(mk_selected_no_ammo_color);

        else
            p_arr_img[(int)current_selected_weapon]->SetColorAndOpacity(mk_selected_color);
    }
}

void UCurrent_Weapon_UI::Set_icon_UI()
{
    if (!mp_weapon_manager &&
        !mp_UI_manager)
        return;
    
    // ù��° ����
    if (mp_weapon_manager->arr_is_weapon_equipped[0])
        First_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_first_gun->weapon_type]);

    // �ι�° ����
    if (mp_weapon_manager->arr_is_weapon_equipped[1])
        Second_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_second_gun->weapon_type]);

    // ����° ����
    if (mp_weapon_manager->arr_is_weapon_equipped[2])
        Third_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[(int)mp_weapon_manager->p_pistol->weapon_type]);
    
    //// �׹��� ����
    //if (mp_weapon_manager->arr_is_weapon_equipped[3])
    //{
    //    index = ((int)e_weapon_type::MAX) + ((int)mp_weapon_manager->p_melee->weapon_type);
    //    Fourth_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[index]);
    //}
    //// �ټ���° ����
    //if (mp_weapon_manager->arr_is_weapon_equipped[4])
    //{
    //    index = ((int)e_weapon_type::MAX) + ((int)mp_weapon_manager->p_throwable->weapon_type);
    //    Fifth_weapon_img->SetBrushFromMaterial(mp_UI_manager->map_main_weapon_ui_mat[index]);
    //}
}