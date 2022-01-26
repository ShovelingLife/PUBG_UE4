#include "Inventory_Weapon_Slot_UI.h"
#include "UI_manager.h"
#include "Components/CanvasPanelSlot.h"
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
    Update_weapon_UI_highlight_img();
}

void UInventory_Weapon_Slot_UI::NativeOnMouseEnter(const FGeometry& _geometry, const FPointerEvent& _mouse_event)
{
    Super::NativeOnMouseEnter(_geometry, _mouse_event);
    //GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, _geometry.ToString());
}

void UInventory_Weapon_Slot_UI::NativeOnMouseLeave(const FPointerEvent& _mouse_event)
{
    Super::NativeOnMouseLeave(_mouse_event);
    Highlight_img->SetVisibility(ESlateVisibility::Hidden);
    m_selected_weapon_index = 0;
}

FReply UInventory_Weapon_Slot_UI::NativeOnMouseButtonDown(const FGeometry& _geometry, const FPointerEvent& _mouse_event)
{
    Super::NativeOnMouseButtonDown(_geometry, _mouse_event);
    
    // 왼쪽 클릭 / 마우스 커서 변경
    if(_mouse_event.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        switch(m_selected_weapon_index)
        {
        case 1: 
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;
        }
    }
    // 오른쪽 클릭 / 맵에다가 무기를 버림
    else if (_mouse_event.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        ABase_interaction* p_tmp_weapon = nullptr;

        switch (m_selected_weapon_index)
        {
        case 1: p_tmp_weapon = mp_weapon_manager->p_first_gun;  break;
        case 2: p_tmp_weapon = mp_weapon_manager->p_second_gun; break;
        case 3: p_tmp_weapon = mp_weapon_manager->p_pistol;     break;
        case 4: p_tmp_weapon = mp_weapon_manager->p_melee;      break;
        case 5: p_tmp_weapon = mp_weapon_manager->p_throwable;  break;
        }
        mp_weapon_manager->Drop(p_tmp_weapon);
    }

    return FReply::Handled();
}

void UInventory_Weapon_Slot_UI::Update_UI_visibility()
{
    // 첫번째 무기
    First_gun_slot_img->SetVisibility((mp_weapon_manager->p_first_gun)        ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_name_txt->SetVisibility((mp_weapon_manager->p_first_gun)        ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_bullet_type_txt->SetVisibility((mp_weapon_manager->p_first_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_magazine_txt->SetVisibility((mp_weapon_manager->p_first_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_number_txt->SetVisibility((mp_weapon_manager->p_first_gun)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 두번째 무기
    Second_gun_slot_img->SetVisibility((mp_weapon_manager->p_second_gun)        ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_name_txt->SetVisibility((mp_weapon_manager->p_second_gun)        ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_bullet_type_txt->SetVisibility((mp_weapon_manager->p_second_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_magazine_txt->SetVisibility((mp_weapon_manager->p_second_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_number_txt->SetVisibility((mp_weapon_manager->p_second_gun)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 세번째 무기
    Pistol_slot_img->SetVisibility((mp_weapon_manager->p_pistol)        ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_name_txt->SetVisibility((mp_weapon_manager->p_pistol)        ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_bullet_type_txt->SetVisibility((mp_weapon_manager->p_pistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_magazine_txt->SetVisibility((mp_weapon_manager->p_pistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_number_txt->SetVisibility((mp_weapon_manager->p_pistol)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 네번째 무기
    Melee_slot_img->SetVisibility((mp_weapon_manager->p_melee)   ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Melee_name_txt->SetVisibility((mp_weapon_manager->p_melee)   ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Melee_number_txt->SetVisibility((mp_weapon_manager->p_melee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 다섯번째 무기
    Grenade_slot_img->SetVisibility((mp_weapon_manager->p_throwable)   ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Grenade_name_txt->SetVisibility((mp_weapon_manager->p_throwable)   ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Grenade_number_txt->SetVisibility((mp_weapon_manager->p_throwable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInventory_Weapon_Slot_UI::Update_inventory_weapon_UI()
{
    // 첫번째 무기
    if(mp_weapon_manager->arr_is_weapon_equipped[0])
    {
        auto p_first_gun = mp_weapon_manager->p_first_gun;
        First_gun_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)p_first_gun->weapon_type]));
        First_gun_name_txt->SetText(FText::FromString(p_first_gun->weapon_data.type));        
    }
    // 두번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[1])
    {
        auto p_second_gun = mp_weapon_manager->p_second_gun;
        Second_gun_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)p_second_gun->weapon_type]));
        Second_gun_name_txt->SetText(FText::FromString(p_second_gun->weapon_data.type));
    }
    // 세번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[2])
    {
        auto p_pistol = mp_weapon_manager->p_pistol;
        Pistol_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_pistol->weapon_type]));
        Pistol_name_txt->SetText(FText::FromString(p_pistol->weapon_data.type));
    }
    // 네번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[3])
    {
        auto p_throwable = mp_weapon_manager->p_throwable;
        Melee_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_throwable->weapon_type]));
        Melee_name_txt->SetText(FText::FromString(p_throwable->weapon_data.type));
    }
    // 다섯번째 무기
    if (mp_weapon_manager->arr_is_weapon_equipped[4])
    {
        auto p_melee = mp_weapon_manager->p_melee;
        Grenade_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_melee->weapon_type]));
        Grenade_name_txt->SetText(FText::FromString(p_melee->weapon_data.type));
    }
}

void UInventory_Weapon_Slot_UI::Update_weapon_UI_highlight_img()
{
    float size_x = 0.f;    

    // 선택된 무기 인덱스 구함
    if      (First_gun_slot_img->IsHovered())
             m_selected_weapon_index = 1;

    else if (Second_gun_slot_img->IsHovered())
             m_selected_weapon_index = 2;

    else if (Pistol_slot_img->IsHovered())
             m_selected_weapon_index = 3;

    else if (Melee_slot_img->IsHovered())
             m_selected_weapon_index = 4;

    else if (Grenade_slot_img->IsHovered())
             m_selected_weapon_index = 5;

    // 무기가 선택됐을 시 이미지 설정
    switch(m_selected_weapon_index)
    {
    case 1: First_gun_canvas_panel->AddChild(Highlight_img);  size_x = 610.f; break;
    case 2: Second_gun_canvas_panel->AddChild(Highlight_img); size_x = 610.f; break;
    case 3: Pistol_canvas_panel->AddChild(Highlight_img);     size_x = 610.f; break;
    case 4: Melee_canvas_panel->AddChild(Highlight_img);      size_x = 290.f; break;
    case 5: Grenade_canvas_panel->AddChild(Highlight_img);    size_x = 290.f; break;
    }
    if(m_selected_weapon_index != 0)
    {
        auto canvas_panel_slot = Cast<UCanvasPanelSlot>(Highlight_img->Slot);
        canvas_panel_slot->SetSize(FVector2D(size_x, 170.f));
        canvas_panel_slot->SetPosition(FVector2D(16.f, 80.f));
        Highlight_img->SetVisibility(ESlateVisibility::Visible);
    }
}