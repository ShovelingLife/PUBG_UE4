#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "PUBG_UE4/UtilityData.h"
#include "CoreBackpack.generated.h"

class UStaticMesh;

UENUM()
enum class EBackpackType
{
	FIRST  = 2,
	SECOND = 4,
	THIRD  = 7
};

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