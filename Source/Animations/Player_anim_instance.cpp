#include "Player_anim_instance.h"
#include "Characters/Custom_player.h"

UPlayer_anim_instance::UPlayer_anim_instance()
{

}

void UPlayer_anim_instance::NativeUpdateAnimation(float _delta_seconds)
{
    Super::NativeUpdateAnimation(_delta_seconds);

    APawn* owning_pawn = TryGetPawnOwner();
    ACustom_player* player = Cast<ACustom_player>(owning_pawn);
    //AAI_character* ai = Cast<AAI_character>(owning_pawn);

    // 캐릭터가 널이 아닐 시
    if (player)
    {
        //// 값 적용Set values
        //player_state = (e_player_animation_state)player->current_state;
        //speed = player->GetCharacterMovement()->MaxWalkSpeed + player->GetInputAxisValue(TEXT("Up_down"));
        //is_weapon_equipped = player->is_weapon_equipped;
    }
    //// AI가 널이 아닐 시
    //else if (ai)
    //{
    //    GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("Enemy animation entered : %d"), player_state));
    //    // Set values
    //    player_state = ai->current_state;
    //    speed = ai->GetCharacterMovement()->MaxWalkSpeed + ai->GetInputAxisValue(TEXT("Up_down"));
    //    is_weapon_equipped = ai->is_weapon_equipped;
    //}
}