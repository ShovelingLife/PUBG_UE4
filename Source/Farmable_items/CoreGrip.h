#pragma once

#include "CoreMinimal.h"
#include "CoreAttachment.h"
#include "CoreGrip.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACoreGrip : public ACoreAttachment
{
	GENERATED_BODY()
	
public:
	// ���ذ� ���� ��
	float RecoilReduceVal = 0.f;
};