#pragma once

#include "CoreMinimal.h"
#include "CoreAttachment.h"
#include "CoreSight.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACoreSight : public ACoreAttachment
{
	GENERATED_BODY()

public:
    // ���ذ� ���� ��
    float ZoomVal = 0.f;
};