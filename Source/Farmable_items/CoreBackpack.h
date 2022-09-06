#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "PUBG_UE4/UtilityData.h"
#include "PUBG_UE4/UtilityEnum.h"
#include "CoreBackpack.generated.h"

class UStaticMesh;


UCLASS()
class FARMABLE_ITEMS_API ACoreBackpack : public ACoreFarmableItem
{
	GENERATED_BODY()	

public:
	FsUtilityData Data;

public:
	ACoreBackpack() = default;

	ACoreBackpack(EBackpackType BackpackType);
};