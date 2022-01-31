#include "Inventory_list_UI.h"
#include "Custom_drag_drop_operation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UInventory_list_UI::NativeTick(const FGeometry& _geometry, float _delta_time)
{
    Super::NativeTick(_geometry, _delta_time);
}

FReply UInventory_list_UI::NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&)
{
    return FReply::Handled();
}

FReply UInventory_list_UI::NativeOnMouseButtonUp(const FGeometry&, const FPointerEvent&)
{
    return FReply::Handled();
}

void UInventory_list_UI::NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _pointer_event, UDragDropOperation*& _operation)
{
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
    FGeometry inventory_size_box_geometry  = Inventory_list_view->GetCachedGeometry();
    FVector2D inventory_size_box_pixel_pos = FVector2D::ZeroVector;
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), inventory_size_box_geometry, FVector2D::ZeroVector, inventory_size_box_pixel_pos, dummy_vector);
    FVector2D inventory_size_box_half_size  = (USlateBlueprintLibrary::GetAbsoluteSize(inventory_size_box_geometry) / FVector2D(2.f));
    FVector2D inventory_size_box_center_pos = inventory_size_box_pixel_pos + inventory_size_box_half_size;

    // 슬롯 이미지 중앙점 구함
    FGeometry slot_image_geometry  = p_slot_UI->Item_img->GetCachedGeometry();
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

    // 월드 리스트에 드롭
    if (image_world_size_box_distance < image_inventory_size_box_distance)
    {
        GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, "Dropped in world");
        // 무기일 시 개수 텍스트 숨김
        if (p_drag_operation->is_weapon)
            p_slot_UI->Name_txt->SetVisibility(ESlateVisibility::Hidden);

        World_list_view->AddItem(p_slot_UI);
        dele_set_weapon_slot_null.ExecuteIfBound();
    }
    // 인벤토리 리스트에 드롭
    else
    {
        GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, "Dropped in inventory");
        if (p_drag_operation->is_weapon)
            return false;

        Inventory_list_view->AddItem(p_slot_UI);
    }
    return true;
}