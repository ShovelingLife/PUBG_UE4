#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "PUBG_UE4/WeaponAttachmentData.h"
#include "FarmableItemEnum.h"
#include "CoreAttachment.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACoreAttachment : public ACoreFarmableItem
{
	GENERATED_BODY()
	
public:
    UPROPERTY()
    UTexture* CurrentItemTex;

    FsWeaponAttachmentData WeaponAttachmentData;
	EWeaponAttachmentType  WeaponAttachmentType;

protected:
    void Init(EWeaponAttachmentType Type);
};