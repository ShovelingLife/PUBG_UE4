#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PlayerAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATIONS_API UPlayerAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
    virtual void NotifyBegin(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

    virtual void NotifyEnd(USkeletalMeshComponent*, UAnimSequenceBase*) override;
};