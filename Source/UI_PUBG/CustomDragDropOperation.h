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

class UItemSlotUI;

UCLASS()
class UI_PUBG_API UCustomDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UItemSlotUI* mCurrentSlot = nullptr;

	/** \brief 슬롯 데이터 관련 */
    bool		 bInventoryList = false;
    bool		 bWeaponSlot	= false;
	bool		 bGun			= false;

public:
	// 초기화 함수
	void Init(UItemSlotUI* pSlot, FString Type = "");

public:
	UItemSlotUI* GetSlot() const;
};