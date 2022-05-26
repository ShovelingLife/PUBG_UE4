#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/BaseInteraction.h"
#include "PUBG_UE4/WeaponAttachmentData.h"
#include "FarmableItemEnum.h"
#include "CoreAttachment.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACoreAttachment : public ABaseInteraction
{
	GENERATED_BODY()
	
public:
    FsWeaponAttachmentData WeaponAttachmentData;
	EWeaponAttachmentType  WeaponAttachmentType;

protected:
    void Init(EWeaponAttachmentType Type);
};