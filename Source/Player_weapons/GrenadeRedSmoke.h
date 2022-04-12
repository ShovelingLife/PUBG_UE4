#pragma once

#include "CoreMinimal.h"
#include "CoreThrowableWeapon.h"
#include "GrenadeRedSmoke.generated.h"

/**
 * 
 */
UCLASS()
class PLAYER_WEAPONS_API AGrenadeRedSmoke : public ACoreThrowableWeapon
{
	GENERATED_BODY()

public:
	AGrenadeRedSmoke();

protected:
	virtual void Explode() override;
};