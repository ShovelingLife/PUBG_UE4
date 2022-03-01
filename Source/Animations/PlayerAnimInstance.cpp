#include "PlayerAnimInstance.h"
#include "Characters/CustomPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{

}

void UPlayerAnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
    Super::NativeUpdateAnimation(_DeltaSeconds);

    ACustomPlayer* p_player = Cast<ACustomPlayer>(TryGetPawnOwner());
    //AAI_character* ai = Cast<AAI_character>(owning_pawn);

    // 캐릭터가 널이 아닐 시
    if (p_player)
    {
        // 값 적용
        if (p_player->bInVehicle)
            PlayerState = EPlayerAnimationState::IDLE;

        else
            PlayerState = (EPlayerAnimationState)p_player->CurrentState;
        
        Speed           = p_player->GetCharacterMovement()->MaxWalkSpeed + p_player->GetInputAxisValue(TEXT("UpDown"));
        bWeaponEquipped = p_player->bWeaponEquipped;
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