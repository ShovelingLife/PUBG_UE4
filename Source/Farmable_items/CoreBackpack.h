#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
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
	
protected:
	TArray<UStaticMesh*> mArrTex;

public:
	int Capacity = 0;

public:
	ACoreBackpack() = default;

	ACoreBackpack(EBackpackType BackpackType);
};