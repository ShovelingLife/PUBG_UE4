#include "PlayerAnimNotifyState.h"
#include "Characters/CustomPlayer.h"

void UPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation, float _TotalDuration)
{
    Super::NotifyBegin(_MeshComp, _Animation, _TotalDuration);

    ACustomPlayer* p_player = Cast<ACustomPlayer>(_MeshComp->GetOwner());

    if (p_player)
        p_player->bAnimationPlaying = true;
}

void UPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation)
{
    Super::NotifyEnd(_MeshComp, _Animation);

    ACustomPlayer* p_player = Cast<ACustomPlayer>(_MeshComp->GetOwner());

    if (p_player)
        p_player->bAnimationPlaying = false;
}