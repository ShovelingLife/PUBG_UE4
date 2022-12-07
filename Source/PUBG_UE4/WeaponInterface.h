#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class PUBG_UE4_API IWeaponInterface
{
	GENERATED_BODY()

protected:
    virtual void ResetSettings();

public:
	// Ŭ�� �̺�Ʈ
    virtual void ClickEvent();
};