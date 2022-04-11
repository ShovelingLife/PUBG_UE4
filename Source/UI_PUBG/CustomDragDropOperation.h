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
	/** \brief 슬롯 데이터 관련 */
    UItemSlotUI*   pSlotUI   = nullptr;
    FsSlotItemData ItemData;
	bool		   bFromList = false;
	bool		   bGun		 = false;
};
