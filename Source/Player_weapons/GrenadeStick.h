#pragma once

#include "CoreMinimal.h"
#include "CoreThrowableWeapon.h"
#include "GrenadeStick.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_WEAPONS_API AGrenadeStick : public ACoreThrowableWeapon
{
	GENERATED_BODY()
	
public:
	AGrenadeStick();

protected:
	virtual void Explode() override;
};