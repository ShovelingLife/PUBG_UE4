#pragma once

#include "CoreMinimal.h"
#include "CoreThrowableWeapon.h"
#include "GrenadeFragmentation2.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_WEAPONS_API AGrenadeFragmentation2 : public ACoreThrowableWeapon
{
	GENERATED_BODY()
	
public:
	AGrenadeFragmentation2();

protected:
	virtual void Explode() override;
};