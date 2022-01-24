#include "Inventory_Weapon_Slot_UI.h"
#include "UI_manager.h"
#include "Characters/Custom_player.h"
#include "Kismet/GameplayStatics.h"
#include "Player_weapons/Core_weapon.h"
#include "Player_weapons/Core_melee_weapon.h"
#include "Player_weapons/Core_throwable_weapon.h"
#include "Player_weapons/Weapon_manager.h"

void UInventory_Weapon_Slot_UI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventory_Weapon_Slot_UI::NativeTick(const FGeometry& _my_geometry, float _delta_time)
{
    Super::NativeTick(_my_geometry, _delta_time);

    if(!mp_weapon_manager)
        mp_weapon_manager = Cast<ACustom_player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->Get_weapon_manager();

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    Update_UI_visibility();
    Update_inventory_weapon_UI();
}

void UInventory_Weapon_Slot_UI::Update_UI_visibility()
{
    // 첫번째 무기
    Gun_slot1_img->SetVisibility((mp_weapon_manager->p_first_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Gun_name1_txt->SetVisibility((mp_weapon_manager->p_first_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Bullet_type1_txt->SetVisibility((mp_weapon_manager->p_first_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Magazine1_txt->SetVisibility((mp_weapon_manager->p_first_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Number1_txt->SetVisibility((mp_weapon_manager->p_first_gun)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 두번째 무기
    Gun_slot2_img->SetVisibility((mp_weapon_manager->p_second_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Gun_name2_txt->SetVisibility((mp_weapon_manager->p_second_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Bullet_type2_txt->SetVisibility((mp_weapon_manager->p_second_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Magazine2_txt->SetVisibility((mp_weapon_manager->p_second_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Number2_txt->SetVisibility((mp_weapon_manager->p_second_gun)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 세번째 무기
    Gun_slot3_img->SetVisibility((mp_weapon_manager->p_pistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Gun_name3_txt->SetVisibility((mp_weapon_manager->p_pistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Bullet_type3_txt->SetVisibility((mp_weapon_manager->p_pistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Magazine3_txt->SetVisibility((mp_weapon_manager->p_pistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Number3_txt->SetVisibility((mp_weapon_manager->p_second_gun)  ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 네번째 무기
    Gun_slot4_img->SetVisibility((mp_weapon_manager->p_melee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Gun_name4_txt->SetVisibility((mp_weapon_manager->p_melee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Number4_txt->SetVisibility((mp_weapon_manager->p_melee)   ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 다섯번째 무기
    Gun_slot5_img->SetVisibility((mp_weapon_manager->p_throwable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Gun_name5_txt->SetVisibility((mp_weapon_manager->p_throwable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Number5_txt->SetVisibility((mp_weapon_manager->p_throwable)   ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInventory_Weapon_Slot_UI::Update_inventory_weapon_UI()
{
    // 첫번째 무기
    if(mp_weapon_manager->arr_is_weapon_equipped[0])
    {
        auto p_first_gun = mp_weapon_manager->p_first_gun;
        Gun_slot1_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)p_first_gun->weapon_type]));
        Gun_name1_txt->SetText(FText::FromString(p_first_gun->weapon_data.type));        
    }
    // 두번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[1])
    {
        auto p_second_gun = mp_weapon_manager->p_second_gun;
        Gun_slot2_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)p_second_gun->weapon_type]));
        Gun_name2_txt->SetText(FText::FromString(p_second_gun->weapon_data.type));
    }
    // 세번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[2])
    {
        auto p_pistol = mp_weapon_manager->p_pistol;
        Gun_slot3_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_pistol->weapon_type]));
        Gun_name3_txt->SetText(FText::FromString(p_pistol->weapon_data.type));
    }
    // 네번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[3])
    {
        auto p_throwable = mp_weapon_manager->p_throwable;
        Gun_slot4_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_throwable->weapon_type]));
        Gun_name4_txt->SetText(FText::FromString(p_throwable->weapon_data.type));
    }
    // 다섯번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[4])
    {
        auto p_melee = mp_weapon_manager->p_melee;
        Gun_slot5_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_melee->weapon_type]));
        Gun_name5_txt->SetText(FText::FromString(p_melee->weapon_data.type));
    }
}