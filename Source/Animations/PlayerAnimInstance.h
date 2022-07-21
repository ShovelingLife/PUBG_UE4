/**
 * \file Player_anim_instance.h
 *
 * \brief 플레이어 애니메이션 관련
 *
 * \ingroup Animations
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PUBG_UE4/MyEnum.h"
#include "PlayerAnimInstance.generated.h"


UCLASS()
class ANIMATIONS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerProperty") EPlayerState PlayerState;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariable") float Direction = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariable") float Speed     = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariable") float AimRotPitch = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerVariable") float AimRotYaw = 0.f;

    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
    float crouch_walk_speed = 0.f;*/

    bool bEquipped;

public:
    UPlayerAnimInstance();

public:
    virtual void NativeUpdateAnimation(float) override;
};