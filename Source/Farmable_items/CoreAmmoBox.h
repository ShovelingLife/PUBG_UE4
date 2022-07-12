#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "PUBG_UE4/WeaponBulletData.h"
#include "CoreAmmoBox.generated.h"

class UTexture;

UCLASS()
class FARMABLE_ITEMS_API ACoreAmmoBox : public ACoreFarmableItem
{
	GENERATED_BODY()
	
public:
	FsWeaponBulletData WeaponBulletData;
	EAmmoBoxType	   AmmoBoxType;

protected:
	void Init(EAmmoBoxType Type);
};