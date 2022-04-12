#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceExplodeFunc.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaceExplodeFunc : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PLAYER_WEAPONS_API IInterfaceExplodeFunc
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	virtual void Explode() { }
};
