/**
 * \file Custom_drag_drop_operation.h
 *
 * \brief 드래그 앤 드롭
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item_Slot_UI.h"
#include "Engine/Texture2D.h"
#include "Custom_drag_drop_operation.generated.h"

UCLASS()
class UI_PUBG_API UCustom_drag_drop_operation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	TWeakObjectPtr<UItem_Slot_UI> wk_p_slot_obj;

	/**
	 * \brief 슬롯 데이터 관련
	 */
    UItem_Slot_UI*	  p_slot_UI = nullptr;
    Fs_slot_item_data item_data;
	bool			  is_from_list = false;
	bool			  is_gun	= false;
};
