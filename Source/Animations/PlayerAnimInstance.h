/**
 * \file Player_anim_instance.h
 *
 * \brief �÷��̾� �ִϸ��̼� ����
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
 * \brief �÷��̾� �ִϸ��̼� Ÿ��
 * IDLE ������ ���ֱ�
 * WALK �ȱ�
 * SPRINT �ٱ�
 * JUMP �����ϱ�
 * CROUCH ���̱�
 * PRONING ���帮��
 * AIM �����ϱ� (������)
 * THROW ������
 * SWIM �����ϱ�
 * INJURED �λ�
 * DEAD ���
 * CROUCH_WALK ����ä �ȱ�
 * PRONING_WALK ���帰ä �ȱ�
 * AIM_WALK ������ä �ȱ�
 * SPRINT_JUMP �� �� ����
 * CROUCH_AIM �ɾƽ�
 * PRONING_AIM �������
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