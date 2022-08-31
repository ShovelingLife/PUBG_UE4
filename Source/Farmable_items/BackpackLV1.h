#pragma once

#include "CoreMinimal.h"
#include "CoreBackpack.h"
#include "BackpackLV1.generated.h"

/**
 * 
 */
UCLASS()
class FARMABLE_ITEMS_API ABackpackLV1 : public ACoreBackpack
{
	GENERATED_BODY()
	
public:
	ABackpackLV1() : Super(EBackpackType::FIRST)
	{
	}
};