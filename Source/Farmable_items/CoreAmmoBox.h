#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "CoreAmmoBox.generated.h"

class UTexture;

UCLASS()
class FARMABLE_ITEMS_API ACoreAmmoBox : public ACoreFarmableItem
{
	GENERATED_BODY()
	
public:
	EAmmoBoxType AmmoBoxType;

protected:
	void Init(EAmmoBoxType Type);
};
