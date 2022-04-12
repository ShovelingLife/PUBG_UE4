#pragma once

#include "CoreMinimal.h"
#include "CoreThrowableWeapon.h"
#include "GrenadeIllumination.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_WEAPONS_API AGrenadeIllumination : public ACoreThrowableWeapon
{
	GENERATED_BODY()

public:
	AGrenadeIllumination();
	
protected:
	virtual void Explode() override;
};