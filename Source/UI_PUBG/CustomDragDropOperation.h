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
#include "ItemSlotUI.h"
#include "Engine/Texture2D.h"
#include "CustomDragDropOperation.generated.h"

UCLASS()
class UI_PUBG_API UCustomDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	TWeakObjectPtr<UItemSlotUI> wk_p_slot_obj;

	/**
	 * \brief 슬롯 데이터 관련
	 */
    UItemSlotUI*	pSlotUI   = nullptr;
    Fs_SlotItemData ItemData;
	bool			bFromList = false;
	bool			bGun	  = false;
};
