#pragma once

#include "CoreMinimal.h"
#include "CoreBackpack.h"
#include "BackpackLV3.generated.h"

/**
 * 
 */
UCLASS()
class FARMABLE_ITEMS_API ABackpackLV3 : public ACoreBackpack
{
	GENERATED_BODY()
	
public:
	ABackpackLV3() : ACoreBackpack(EBackpackType::THIRD)
	{

	}
};