#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/WeaponAttachmentData.h"
#include "FarmableItemEnum.h"
#include "CoreFarmableItem.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACoreFarmableItem : public ABaseInteraction
{
	GENERATED_BODY()
	
public:

public:	
	ACoreFarmableItem();

protected:
	virtual void BeginPlay() override;

	virtual void ResetSettings() final;

public:	
	virtual void Tick(float DeltaTime) override;
};