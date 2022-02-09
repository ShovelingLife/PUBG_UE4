#include "Inventory_list_UI.h"
#include "Item_slot_UI.h"
#include "Custom_drag_drop_operation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UInventory_list_UI::NativeConstruct()
{
    Super::NativeConstruct();
    Get_item_list_width();
}

void UInventory_list_UI::NativeTick(const FGeometry& _geometry, float _delta_time)
{
    Super::NativeTick(_geometry, _delta_time);
}

FReply UInventory_list_UI::NativeOnMouseButtonDown(const FGeometry& _geometry, const FPointerEvent& _mouse_event)
{
    Super::NativeOnMouseButtonDown(_geometry, _mouse_event);

    // 왼쪽 클릭 / 마우스 커서 변경
    if (_mouse_event.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        Highlight_img->SetVisibility(ESlateVisibility::Hidden);

        if (p_slot_obj &&
            p_slot_obj->IsSelected())
        {
            auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(_mouse_event, this, EKeys::LeftMouseButton);
            return reply.NativeReply;
        }
    }
    return FReply::Handled();
}

FReply UInventory_list_UI::NativeOnMouseMove(const FGeometry& _geometry, const FPointerEvent& _in_mouse_event)
{
    Super::NativeOnMouseMove(_geometry, _in_mouse_event);

    // 움직일 때마다 현재 이미지의 위치를 구함    
    FVector2D distance = Get_distance_between_slot_cursor();
    //GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Distance posX : %f, Distance posY : %f"),distance.X, distance.Y));

    if (World_txt->IsHovered()     ||
        Inventory_txt->IsHovered() ||
        Separator_img->IsHovered() ||
        distance.Y <= 2.5f         ||
        distance.Y >= 55.f)    
        Highlight_img->SetVisibility(ESlateVisibility::Hidden);

    return FReply::Handled();
}

void UInventory_list_UI::NativeOnMouseLeave(const FPointerEvent& _in_mouse_event)
{
    Super::NativeOnMouseLeave(_in_mouse_event);
    Highlight_img->SetVisibility(ESlateVisibility::Hidden);
}

void UInventory_list_UI::NativeOnDragDetected(const FGeometry& _geometry, const FPointerEvent& _mouse_event, UDragDropOperation*& _out_operation)
{
    Super::NativeOnDragDetected(_geometry, _mouse_event, _out_operation);
    
    // 마우스 위치를 구함
    auto p_slot         = CreateWidget<UItem_Slot_UI>(GetWorld(), p_item_slot_UI_class);
    FVector2D mouse_pos = _geometry.AbsoluteToLocal(_mouse_event.GetScreenSpacePosition()) + FVector2D(-25.f);

    if (!p_slot ||
        !p_slot_obj)
        return;

    // 슬롯 설정
    p_slot->item_data = p_slot_obj->item_data;
    p_slot->Priority  = 1;
    p_slot->Set_as_cursor(mouse_pos);

    // 드래그 구현
    auto p_drag_drop_operation = NewObject<UCustom_drag_drop_operation>();
    p_drag_drop_operation->p_slot_UI         = p_slot;
    p_drag_drop_operation->DefaultDragVisual = p_slot;
    p_drag_drop_operation->Pivot             = EDragPivot::MouseDown;
    p_drag_drop_operation->item_data         = p_slot_obj->item_data;
    _out_operation  = p_drag_drop_operation;
}

bool UInventory_list_UI::NativeOnDrop(const FGeometry& _geometry, const FDragDropEvent& _pointer_event, UDragDropOperation* _operation)
{
    auto p_drag_operation = Cast<UCustom_drag_drop_operation>(_operation);
    auto p_slot_UI        = p_drag_operation->p_slot_UI;

    if (!p_slot_UI)
        return false;

    // 마우스 위치 구하기
    FVector2D mouse_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 월드 리스트에 드롭
    if (mouse_pos.X > 100.f &&
        mouse_pos.X < 325.f)
    {
        p_slot_UI->dele_check_for_slot.BindUFunction(this, "Check_for_hovered_item");
        World_list_view->AddItem(p_slot_UI);
        dele_set_weapon_slot_null.ExecuteIfBound();
    }
    // 인벤토리 리스트에 드롭
    else
    {
        if (p_drag_operation->is_gun)
            return false;

        p_slot_UI->dele_check_for_slot.BindUFunction(this, "Check_for_hovered_item");
        Inventory_list_view->AddItem(p_slot_UI);
        dele_set_weapon_slot_null.ExecuteIfBound();
    }
    return true;
}

void UInventory_list_UI::Get_item_list_width()
{
    // 월드 사이즈 박스 넓이 구함
    if (auto p_world_list_canvas_slot = Cast<UCanvasPanelSlot>(World_list_size_box->Slot))
    {
        FVector2D world_size_box_pos = p_world_list_canvas_slot->GetPosition();
        FVector2D world_size_box_size = p_world_list_canvas_slot->GetSize();
        m_world_size_box_width = world_size_box_pos.X + world_size_box_size.X;
    }
    // 인벤토리 사이즈 박스 넓이 구함    
    if (auto p_inventory_list_canvas_slot = Cast<UCanvasPanelSlot>(Inventory_list_size_box->Slot))
    {
        FVector2D inventory_size_box_pos = p_inventory_list_canvas_slot->GetPosition();
        FVector2D inventory_size_box_size = p_inventory_list_canvas_slot->GetSize();
        m_inventory_size_box_width = inventory_size_box_pos.X + inventory_size_box_size.X;
    }
}

FVector2D UInventory_list_UI::Get_distance_between_slot_cursor()
{
    // 마우스 위치를 구함   
    FVector2D mouse_pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    if (p_slot_obj)
    {
        // 현재 이미지 위치를 구함
        FVector2D img_pos = FVector2D::ZeroVector, dummy_vec;
        auto cached_geometry = p_slot_obj->GetCachedGeometry();
        USlateBlueprintLibrary::LocalToViewport(GetWorld(), cached_geometry, FVector2D::ZeroVector, dummy_vec, img_pos);

        // 마우스 좌표 - 이미지 좌표 간 거리
        float posX = 0.f, posY = 0.f;

        if (img_pos.X > mouse_pos.X)
            posX = img_pos.X - mouse_pos.X;

        else
            posX = mouse_pos.X - img_pos.X;

        if (img_pos.Y > mouse_pos.Y)
            posY = img_pos.Y - mouse_pos.Y;

        else
            posY = mouse_pos.Y - img_pos.Y;

        return FVector2D(posX, posY);
    }
    return FVector2D::ZeroVector;
}

// this가 넘어오므로 널 체크 불필요
void UInventory_list_UI::Check_for_hovered_item(UItem_Slot_UI* _p_slot_obj)
{
    if (World_list_view->GetIndexForItem(_p_slot_obj) != -1)
        GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Cyan, "Found slot");

    // 현재 이미지 위치를 구함
    FVector2D move_pos = FVector2D::ZeroVector, dummy_vec;
    auto cached_geometry = Cast<UItem_Slot_UI>(_p_slot_obj)->GetCachedGeometry();
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cached_geometry, FVector2D::ZeroVector, dummy_vec, move_pos);
    move_pos.X = 0.f;
    //GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Red, FString::SanitizeFloat(move_pos.Y));

    if (move_pos.Y > 87.f)
        move_pos.Y = move_pos.Y + 5;
        //move_pos.Y = FMath::Abs(move_pos.Y + 4);

    if (auto p_canvas_panel_slot = Cast<UCanvasPanelSlot>(Highlight_img->Slot))
        p_canvas_panel_slot->SetPosition(move_pos);

    Highlight_img->SetVisibility(ESlateVisibility::Visible);
    p_slot_obj = _p_slot_obj;
}