﻿ #include "Inventory_Weapon_Slot_UI.h"
#include "Item_slot_UI.h"
#include "Inventory_UI.h"
#include "Inventory_list_UI.h"
#include "Custom_drag_drop_operation.h"
#include "UI_manager.h"
#include "Inventory_manager.h"
#include "Characters/Custom_player.h"
#include "Player_weapons/Core_weapon.h"
#include "Player_weapons/Core_melee_weapon.h"
#include "Player_weapons/Core_throwable_weapon.h"
#include "Player_weapons/Weapon_manager.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInventory_Weapon_Slot_UI::NativeConstruct()
{
    Super::NativeConstruct();   
}

void UInventory_Weapon_Slot_UI::NativeTick(const FGeometry& _my_geometry, float _delta_time)
{
    Super::NativeTick(_my_geometry, _delta_time);

    if (!mp_weapon_manager)
        mp_weapon_manager = Cast<ACustom_player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->Get_weapon_manager();

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));

    Update_UI_visibility();
    Update_inventory_weapon_UI();
    Update_weapon_UI_highlight_img(); 
}

void UInventory_Weapon_Slot_UI::NativeOnMouseLeave(const FPointerEvent& _in_mouse_event)
{
    Super::NativeOnMouseLeave(_in_mouse_event);
    Reset_highlight_img();
}

FReply UInventory_Weapon_Slot_UI::NativeOnMouseButtonDown(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event)
{
    Super::NativeOnMouseButtonDown(_geometry, _in_mouse_event);
    
    // 왼쪽 클릭 / 마우스 커서 변경
    if(_in_mouse_event.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        m_is_clicked = true;
        ABase_interaction* p_tmp_weapon = mp_weapon_manager->Get_weapon((e_current_weapon_type)m_selected_weapon_index);
        int index = 0;

        if(p_tmp_weapon)
        {
            switch (m_selected_weapon_index)
            {
            // 총기류
            case 1: case 2: case 3:
                        
                if (auto p_tmp_gun = Cast<ACore_weapon>(p_tmp_weapon))
                    m_item_data = Fs_slot_item_data(p_tmp_gun->object_type, (int)p_tmp_gun->weapon_type);

                break;

            // 근접 무기
            case 4:

                if (auto p_tmp_melee = Cast<ACore_melee_weapon>(p_tmp_weapon))
                    m_item_data = Fs_slot_item_data(p_tmp_melee->object_type, (int)p_tmp_melee->weapon_type);

                break;

            // 투척류 무기
            case 5:

                if (auto p_tmp_throwable = Cast<ACore_throwable_weapon>(p_tmp_weapon))
                    m_item_data = Fs_slot_item_data(p_tmp_throwable->object_type, (int)p_tmp_throwable->weapon_type);

                break;
            }
        }  
        Highlight_img->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.35f });
    }
    // 오른쪽 클릭 / 맵에다가 무기를 버림
    else if (_in_mouse_event.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        mp_weapon_manager->Drop((e_current_weapon_type)m_selected_weapon_index);
        Reset_highlight_img();
    }
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(_in_mouse_event, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UInventory_Weapon_Slot_UI::NativeOnMouseButtonUp(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event)
{
    Super::NativeOnMouseButtonUp(_geometry, _in_mouse_event);
    m_is_clicked = false;

    Fs_slot_item_data tmp_data;
    m_item_data = tmp_data;

    return FReply::Handled();
}
void UInventory_Weapon_Slot_UI::NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event, UDragDropOperation*& _out_operation)
{
    Super::NativeOnDragDetected(_geometry, _in_mouse_event, _out_operation);

    auto p_slot = CreateWidget<UItem_Slot_UI>(GetWorld(), p_item_slot_UI_class);
    FVector2D mouse_pos = _geometry.AbsoluteToLocal(_in_mouse_event.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (!p_slot)
        return;

    // 슬롯 설정
    p_slot->item_data = m_item_data;
    p_slot->dele_set_weapon_slot_null.BindUFunction(this, "Set_slot_null");
    p_slot->Priority = 1;
    p_slot->Set_as_cursor(mouse_pos);

    // 드래그 구현
    auto p_drag_drop_operation = NewObject<UCustom_drag_drop_operation>();

    if (m_selected_weapon_index < 4)
        p_drag_drop_operation->is_gun = true;

    p_drag_drop_operation->p_slot_UI         = p_slot;
    p_drag_drop_operation->DefaultDragVisual = p_slot;
    p_drag_drop_operation->Pivot             = EDragPivot::MouseDown;
    p_drag_drop_operation->item_data         = m_item_data;
    _out_operation = p_drag_drop_operation;
    Reset_highlight_img();
}

bool UInventory_Weapon_Slot_UI::NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _drag_drop_event, UDragDropOperation* _in_operation)
{
    Super::NativeOnDrop(_geometry, _drag_drop_event, _in_operation);
    m_is_clicked = false;
    auto p_custom_drag_drop_operation = Cast<UCustom_drag_drop_operation>(_in_operation);
    
    if (p_custom_drag_drop_operation->wk_p_slot_obj.Get())
        p_custom_drag_drop_operation->wk_p_slot_obj->MarkPendingKill();

    return true;
}

void UInventory_Weapon_Slot_UI::NativeOnDragLeave(const FDragDropEvent& _in_drag_drop_event, UDragDropOperation* _in_operation)
{
    Super::NativeOnDragLeave(_in_drag_drop_event, _in_operation);
    Reset_highlight_img();
    m_is_clicked = false;
}

void UInventory_Weapon_Slot_UI::Update_UI_visibility()
{
    // 첫번째 무기
    First_gun_slot_img->SetVisibility((mp_weapon_manager->p_first_gun)             ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_name_txt->SetVisibility((mp_weapon_manager->p_first_gun)             ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_bullet_type_txt->SetVisibility((mp_weapon_manager->p_first_gun)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_current_magazine_txt->SetVisibility((mp_weapon_manager->p_first_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_max_magazine_txt->SetVisibility((mp_weapon_manager->p_first_gun)     ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    First_gun_number_background->SetVisibility((mp_weapon_manager->p_first_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 두번째 무기
    Second_gun_slot_img->SetVisibility((mp_weapon_manager->p_second_gun)             ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_name_txt->SetVisibility((mp_weapon_manager->p_second_gun)             ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_bullet_type_txt->SetVisibility((mp_weapon_manager->p_second_gun)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_current_magazine_txt->SetVisibility((mp_weapon_manager->p_second_gun) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_max_magazine_txt->SetVisibility((mp_weapon_manager->p_second_gun)     ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Second_gun_number_background->SetVisibility((mp_weapon_manager->p_second_gun)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 세번째 무기
    Pistol_slot_img->SetVisibility((mp_weapon_manager->p_pistol)             ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_name_txt->SetVisibility((mp_weapon_manager->p_pistol)             ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_bullet_type_txt->SetVisibility((mp_weapon_manager->p_pistol)      ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_current_magazine_txt->SetVisibility((mp_weapon_manager->p_pistol) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_max_magazine_txt->SetVisibility((mp_weapon_manager->p_pistol)     ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Pistol_number_background->SetVisibility((mp_weapon_manager->p_pistol)    ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 네번째 무기
    Melee_slot_img->SetVisibility((mp_weapon_manager->p_melee)          ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Melee_name_txt->SetVisibility((mp_weapon_manager->p_melee)          ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Melee_number_background->SetVisibility((mp_weapon_manager->p_melee) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // 다섯번째 무기
    Grenade_slot_img->SetVisibility((mp_weapon_manager->p_throwable)          ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Grenade_name_txt->SetVisibility((mp_weapon_manager->p_throwable)          ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    Grenade_number_background->SetVisibility((mp_weapon_manager->p_throwable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInventory_Weapon_Slot_UI::Update_inventory_weapon_UI()
{
    if (!mp_UI_manager ||
        !mp_weapon_manager)
        return;

    // 첫번째 무기
    if(auto p_first_gun = mp_weapon_manager->p_first_gun)
    {
        First_gun_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)p_first_gun->weapon_type]));
        First_gun_name_txt->SetText(FText::FromString(p_first_gun->weapon_data.type));        
    }
    // 두번째 무기
    if (auto p_second_gun = mp_weapon_manager->p_second_gun)
    {
        Second_gun_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)p_second_gun->weapon_type]));
        Second_gun_name_txt->SetText(FText::FromString(p_second_gun->weapon_data.type));
    }
    // 세번째 무기
    if (auto p_pistol = mp_weapon_manager->p_pistol)
    {
        Pistol_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_pistol->weapon_type]));
        Pistol_name_txt->SetText(FText::FromString(p_pistol->weapon_data.type));
    }
    // 네번째 무기
    if (auto p_throwable = mp_weapon_manager->p_throwable)
    {
        Melee_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_throwable->weapon_type]));
        Melee_name_txt->SetText(FText::FromString(p_throwable->weapon_data.type));
    }
    // 다섯번째 무기
    if (auto p_melee = mp_weapon_manager->p_melee)
    {
        Grenade_slot_img->SetBrushFromTexture(Cast<UTexture2D>(mp_UI_manager->map_inventory_weapon_ui_tex[(int)mp_weapon_manager->p_melee->weapon_type]));
        Grenade_name_txt->SetText(FText::FromString(p_melee->weapon_data.type));
    }
}

void UInventory_Weapon_Slot_UI::Update_weapon_UI_highlight_img()
{
    if (m_is_clicked)
        return;

    float size_x = 0.f;    

    // 선택된 무기 인덱스 구함
    if      (First_gun_slot_img->GetVisibility() == ESlateVisibility::Visible &&
             First_gun_slot_img->IsHovered())
             m_selected_weapon_index = 1;

    else if (Second_gun_slot_img->GetVisibility() == ESlateVisibility::Visible &&
             Second_gun_slot_img->IsHovered())
             m_selected_weapon_index = 2;

    else if (Pistol_slot_img->GetVisibility() == ESlateVisibility::Visible &&
             Pistol_slot_img->IsHovered())
             m_selected_weapon_index = 3;

    else if (Melee_slot_img->GetVisibility() == ESlateVisibility::Visible &&
             Melee_slot_img->IsHovered())
             m_selected_weapon_index = 4;

    else if (Grenade_slot_img->GetVisibility() == ESlateVisibility::Visible &&
             Grenade_slot_img->IsHovered())
             m_selected_weapon_index = 5;

    UCanvasPanelSlot* canvas_panel_slot = nullptr;

    // 무기가 선택됐을 시 이미지 설정
    switch(m_selected_weapon_index)
    {
    case 1: canvas_panel_slot = First_gun_canvas_panel->AddChildToCanvas(Highlight_img);  size_x = 545.f; break;
    case 2: canvas_panel_slot = Second_gun_canvas_panel->AddChildToCanvas(Highlight_img); size_x = 545.f; break;
    case 3: canvas_panel_slot = Pistol_canvas_panel->AddChildToCanvas(Highlight_img);     size_x = 545.f; break;
    case 4: canvas_panel_slot = Melee_canvas_panel->AddChildToCanvas(Highlight_img);      size_x = 225.f; break;
    case 5: canvas_panel_slot = Grenade_canvas_panel->AddChildToCanvas(Highlight_img);    size_x = 225.f; break;
    }
    if (m_selected_weapon_index != 0 &&
        canvas_panel_slot)
    {
        canvas_panel_slot->SetSize(FVector2D(size_x, 195.f));
        Highlight_img->SetVisibility(ESlateVisibility::Visible);
        Highlight_img->SetColorAndOpacity(FLinearColor{ 1.f,1.f,1.f,0.1f });
    }
}

void UInventory_Weapon_Slot_UI::Reset_highlight_img()
{
    // 선택 이미지 부모 새로 지정 및 무기 슬롯 이미지 설정
    switch (m_selected_weapon_index)
    {
    case 1: First_gun_canvas_panel->RemoveChild(Highlight_img);  break;
    case 2: Second_gun_canvas_panel->RemoveChild(Highlight_img); break;
    case 3: Pistol_canvas_panel->RemoveChild(Highlight_img);     break;
    case 4: Melee_canvas_panel->RemoveChild(Highlight_img);      break;
    case 5: Grenade_canvas_panel->RemoveChild(Highlight_img);    break;
    }
    m_selected_weapon_index = 0;
    Main_canvas_panel->AddChildToCanvas(Highlight_img);
    Highlight_img->SetVisibility(ESlateVisibility::Hidden);
}

void UInventory_Weapon_Slot_UI::Set_slot_null()
{
    GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, "Set_slot_null");
    if (mp_weapon_manager)
        mp_weapon_manager->Set_null((e_current_weapon_type)m_selected_weapon_index);

    m_selected_weapon_index = 0;
    Reset_highlight_img();
}