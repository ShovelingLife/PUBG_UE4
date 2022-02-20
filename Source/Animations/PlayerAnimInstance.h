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
#include "PlayerAnimInstance.generated.h"

/**
 * \brief 플레이어 애니메이션 타입
 * IDLE 가만히 서있기
 * WALK 걷기
 * SPRINT 뛰기
 * JUMP 점프하기
 * CROUCH 숙이기
 * PRONING 엎드리기
 * AIM 조준하기 (서서쏴)
 * THROW 던지기
 * SWIM 수영하기
 * INJURED 부상
 * DEAD 사망
 * CROUCH_WALK 숙인채 걷기
 * PRONING_WALK 엎드린채 걷기
 * AIM_WALK 조준한채 걷기
 * SPRINT_JUMP 뛴 후 점프
 * CROUCH_AIM 앉아쏴
 * PRONING_AIM 엎드려쏴
 * TEST
 */
UENUM(BlueprintType)
enum class EPlayerAnimationState : uint8
{
    IDLE         UMETA(DisplayName = "IDLE"),
    WALK         UMETA(DisplayName = "WALK"),
    SPRINT       UMETA(DisplayName = "SPRINT"),
    JUMP         UMETA(DisplayName = "JUMP"),
    CROUCH       UMETA(DisplayName = "CROUCH"),
    PRONING      UMETA(DisplayName = "PRONING"),
    AIM          UMETA(DisplayName = "AIM"),
    THROW        UMETA(DisplayName = "THROW"),
    SWIM         UMETA(DisplayName = "SWIM"),
    INJURED      UMETA(DisplayName = "INJURED"),
    DEAD         UMETA(DisplayName = "DEAD"),
    CROUCH_WALK  UMETA(DisplayName = "CROUCH_WALKING"),
    PRONING_WALK UMETA(DisplayName = "PRONING_WALKING"),
    AIM_WALK     UMETA(DisplayName = "AIM_WALKING"),
    SPRINT_JUMP  UMETA(DisplayName = "JUMP_ON_SPRINT"),
    CROUCH_AIM   UMETA(DisplayName = "CROUCH_ON_AIM"),
    PRONING_AIM  UMETA(DisplayName = "CROUCH_ON_PRONING")
};

UCLASS()
class ANIMATIONS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_property") EPlayerAnimationState PlayerState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint") float Speed = 0.f;


    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
    float crouch_walk_speed = 0.f;*/

    bool bWeaponEquipped;

public:
    UPlayerAnimInstance();

public:
    virtual void NativeUpdateAnimation(float) override;
};