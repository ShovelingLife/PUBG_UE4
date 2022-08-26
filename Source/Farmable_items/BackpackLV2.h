#pragma once

#include "CoreMinimal.h"
#include "CoreBackpack.h"
#include "BackpackLV2.generated.h"

/**
 * 
 */
UCLASS()
class FARMABLE_ITEMS_API ABackpackLV2 : public ACoreBackpack
{
	GENERATED_BODY()
	
public:
	ABackpackLV2() : Super(EBackpackType::SECOND)
	{

	}
};