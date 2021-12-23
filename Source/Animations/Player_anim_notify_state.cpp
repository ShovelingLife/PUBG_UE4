#include "Player_anim_notify_state.h"
#include "Characters/Custom_player.h"

void UPlayer_anim_notify_state::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    ACustom_player* player = Cast<ACustom_player>(MeshComp->GetOwner());

    /*if (player)
        player->is_animation_playing = true;*/
}

void UPlayer_anim_notify_state::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    ACustom_player* player = Cast<ACustom_player>(MeshComp->GetOwner());

    /*if (player)
        player->is_animation_playing = false;*/
}