#pragma once

#include "CoreMinimal.h"
#include "CoreAttachment.h"
#include "CoreSight.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACoreSight : public ACoreAttachment
{
	GENERATED_BODY()

public:
    // 조준경 배율 값
    float ZoomVal = 0.f;
};