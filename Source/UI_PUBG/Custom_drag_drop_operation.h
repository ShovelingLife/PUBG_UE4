#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "PUBG_UE4/Global.h"
#include "Engine/Texture2D.h"
#include "Custom_drag_drop_operation.generated.h"

class UItem_Slot_UI;

UCLASS()
class UI_PUBG_API UCustom_drag_drop_operation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
    UItem_Slot_UI*	  p_slot_UI = nullptr;
    Fs_slot_item_data item_data;
	bool			  is_weapon	= false;
};
