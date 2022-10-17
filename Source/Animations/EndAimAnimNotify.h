#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndAimAnimNotify.generated.h"

UCLASS()
class ANIMATIONS_API UEndAimAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};