#include "PlayerAnimInstance.h"
#include "Characters/CustomPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    //AAI_character* ai = Cast<AAI_character>(owning_pawn);

    // 캐릭터가 널이 아닐 시
    if (ACustomPlayer* p_player = Cast<ACustomPlayer>(TryGetPawnOwner()))
    {
        FRotator controlRot = p_player->GetControlRotation();
        FRotator actorRot   = p_player->GetActorRotation();
        FRotator normDeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(controlRot, actorRot);
        FRotator currentRot = UKismetMathLibrary::MakeRotator(0.f, AimRotPitch, AimRotYaw);
        FRotator interptRot = UKismetMathLibrary::RInterpTo(currentRot, normDeltaRot, DeltaSeconds, 15.f);
        FVector  velocity   = p_player->GetVelocity();
        const float minAngleDeg = -90.f, maxAngleDeg = 90.f;

        // 값 적용
        PlayerState = (p_player->bInVehicle) ? EPlayerState::IDLE : p_player->CurrentState;
        Direction = UKismetAnimationLibrary::CalculateDirection(velocity, actorRot);
        Speed = velocity.Size();
        //bEquipped = p_player->bWeaponEquipped;

        // 에임 오프셋 적용        
        AimRotPitch = UKismetMathLibrary::ClampAngle(interptRot.Pitch, minAngleDeg, maxAngleDeg);
        AimRotYaw = UKismetMathLibrary::ClampAngle(interptRot.Yaw, minAngleDeg, maxAngleDeg);
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