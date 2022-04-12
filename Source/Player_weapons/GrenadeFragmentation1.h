#pragma once

#include "CoreMinimal.h"
#include "CoreThrowableWeapon.h"
#include "GrenadeFragmentation1.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_WEAPONS_API AGrenadeFragmentation1 : public ACoreThrowableWeapon
{
	GENERATED_BODY()
	
public:
	AGrenadeFragmentation1();

protected:
	virtual void Explode() override;
};