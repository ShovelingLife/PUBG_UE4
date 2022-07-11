#include "PlayerAnimNotifyState.h"
#include "Characters/CustomPlayer.h"

void UPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    ACustomPlayer* p_player = Cast<ACustomPlayer>(MeshComp->GetOwner());

    if (p_player)
        p_player->bAnimationPlaying = true;
}

void UPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    ACustomPlayer* p_player = Cast<ACustomPlayer>(MeshComp->GetOwner());

    if (p_player)
        p_player->bAnimationPlaying = false;
}