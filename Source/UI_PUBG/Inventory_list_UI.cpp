#include "Inventory_list_UI.h"
#include "Item_slot_UI.h"
#include "Custom_drag_drop_operation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UInventory_list_UI::NativeTick(const FGeometry& _geometry, float _delta_time)
{
    Super::NativeTick(_geometry, _delta_time);
    Change_highlight_img_pos();
}

bool UInventory_list_UI::NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, UDragDropOperation* _operation)
{
    auto p_drag_operation = Cast<UCustom_drag_drop_operation>(_operation);
    auto p_slot_UI        = p_drag_operation->p_slot_UI;

    if (!p_slot_UI)
        return false;

    p_slot_UI->item_data = p_drag_operation->item_data;

    FVector2D dummy_vector = FVector2D::ZeroVector;

    // 월드 사이즈 박스 중앙점 위치 구함
    FGeometry world_size_box_geometry  = World_list_size_box->GetCachedGeometry();
    FVector2D world_size_box_pixel_pos = FVector2D::ZeroVector;
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), world_size_box_geometry, FVector2D::ZeroVector, world_size_box_pixel_pos, dummy_vector);
    FVector2D world_size_box_half_size  = (USlateBlueprintLibrary::GetAbsoluteSize(world_size_box_geometry) / FVector2D(2.f));
    FVector2D world_size_box_center_pos = world_size_box_pixel_pos + world_size_box_half_size;

    // 인벤토리 사이즈 박스 중앙점 구함
    FGeometry inventory_size_box_geometry  = Inventory_list_size_box->GetCachedGeometry();
    FVector2D inventory_size_box_pixel_pos = FVector2D::ZeroVector;
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), inventory_size_box_geometry, FVector2D::ZeroVector, inventory_size_box_pixel_pos, dummy_vector);
    FVector2D inventory_size_box_half_size  = (USlateBlueprintLibrary::GetAbsoluteSize(inventory_size_box_geometry) / FVector2D(2.f));
    FVector2D inventory_size_box_center_pos = inventory_size_box_pixel_pos + inventory_size_box_half_size;

    // 슬롯 이미지 중앙점 구함
    FGeometry slot_image_geometry  = p_slot_UI->Main_horizontal_box->GetCachedGeometry();
    FVector2D slot_image_pixel_pos = FVector2D::ZeroVector;
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), slot_image_geometry, FVector2D::ZeroVector, slot_image_pixel_pos, dummy_vector);
    FVector2D slot_image_half_size  = (USlateBlueprintLibrary::GetAbsoluteSize(slot_image_geometry) / FVector2D(2.f));
    FVector2D slot_image_center_pos = slot_image_pixel_pos + slot_image_half_size;

    // 슬롯 이미지 - 월드 리스트 UI 중간값간 거리    
    float image_world_size_box_distance     = FVector::Distance(FVector(slot_image_center_pos.X, slot_image_center_pos.Y, 0.f), 
                                                                FVector(world_size_box_center_pos.X, world_size_box_center_pos.Y, 0.f));

    // 슬롯 이미지 - 인벤토리 리스트 UI 중간값간 거리
    float image_inventory_size_box_distance = FVector::Distance(FVector(slot_image_center_pos.X, slot_image_center_pos.Y, 0.f), 
                                                                FVector(inventory_size_box_center_pos.X, inventory_size_box_center_pos.Y, 0.f));

    GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("image_world_size_box_distance : %f, image_inventory_size_box_distance : %f"), image_inventory_size_box_distance, image_world_size_box_distance));

    // 월드 리스트에 드롭
    if (image_world_size_box_distance < image_inventory_size_box_distance)
    {
        //GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, "World list");
        p_slot_UI->dele_check_for_slot.BindUFunction(this, "Check_for_hovered_item");
        World_list_view->AddItem(p_slot_UI);
        dele_set_weapon_slot_null.ExecuteIfBound();
    }
    // 인벤토리 리스트에 드롭
    else
    {
        //GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, "Inventory list");
        if (p_drag_operation->is_gun)
            return false;

        p_slot_UI->dele_check_for_slot.BindUFunction(this, "Check_for_hovered_item");
        Inventory_list_view->AddItem(p_slot_UI);
        dele_set_weapon_slot_null.ExecuteIfBound();
    }
    return true;
}

void UInventory_list_UI::Check_for_hovered_item(UItem_Slot_UI* _p_slot_obj)
{
    if (m_hovered_list_type == e_hovered_list_type::WORLD ||
        m_hovered_list_type == e_hovered_list_type::INVENTORY)
        return;

    // 월드 리스트 뷰 감시
    int world_list_slot_index = World_list_view->GetIndexForItem(_p_slot_obj);
    
    if(world_list_slot_index != -1)
    {
        World_list_view->GetItemAt(world_list_slot_index);
        m_hovered_list_type = e_hovered_list_type::WORLD;
        wk_p_slot_obj = MakeWeakObjectPtr(_p_slot_obj);
        return;
    }
    // 인벤토리 리스트 뷰 감시
    int inventory_list_slot_index = Inventory_list_view->GetIndexForItem(_p_slot_obj);

    if (inventory_list_slot_index != -1)
    {
        Inventory_list_view->GetItemAt(inventory_list_slot_index);
        m_hovered_list_type = e_hovered_list_type::INVENTORY;
        wk_p_slot_obj = MakeWeakObjectPtr(_p_slot_obj);
    }
}

void UInventory_list_UI::Change_highlight_img_pos()
{
    if (m_hovered_list_type == e_hovered_list_type::NONE)
        return;

    FVector2D move_pos     = FVector2D::ZeroVector;
    FVector2D dummy_vector = FVector2D::ZeroVector;

    // 아이템이 널이 아닐 시 슬롯 접근 후 이미지를 해당 위치로 옮김
    if (auto p_item = Cast<UItem_Slot_UI>(wk_p_slot_obj.Get()))
    {
        if (!p_item->IsHovered())
        {
            Highlight_img->SetVisibility(ESlateVisibility::Hidden);
            m_hovered_list_type = e_hovered_list_type::NONE;
            wk_p_slot_obj->MarkPendingKill();
            return;
        }
        USlateBlueprintLibrary::LocalToViewport(GetWorld(), p_item->GetCachedGeometry(), FVector2D::ZeroVector, dummy_vector, move_pos);
        
        if(auto p_canvas_panel_slot = Cast<UCanvasPanelSlot>(Highlight_img->Slot))
        {
            p_canvas_panel_slot->SetPosition(move_pos);
            Highlight_img->SetVisibility(ESlateVisibility::Visible);
        }
    }
}