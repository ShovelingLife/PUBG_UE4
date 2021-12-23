// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Player_anim_notify_state.generated.h"

/**
 * 
 */
UCLASS()
class ANIMATIONS_API UPlayer_anim_notify_state : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
    virtual void NotifyBegin(USkeletalMeshComponent*, UAnimSequenceBase*, float) override;

    virtual void NotifyEnd(USkeletalMeshComponent*, UAnimSequenceBase*) override;
};
