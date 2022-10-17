#include "PlayerAnimNotifyState.h"
#include "Characters/CustomPlayer.h"

void UPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (ACustomPlayer* p_player = Cast<ACustomPlayer>(MeshComp->GetOwner()))
        p_player->bAnimationPlaying = true;
}

void UPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (ACustomPlayer* p_player = Cast<ACustomPlayer>(MeshComp->GetOwner()))
        p_player->bAnimationPlaying = false;
}