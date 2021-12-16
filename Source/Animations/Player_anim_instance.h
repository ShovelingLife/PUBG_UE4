// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Custom_enum.h"
#include "Animation/AnimInstance.h"
#include "Player_anim_instance.generated.h"

UENUM(BlueprintType)
enum class e_player_animation_state : uint8
{
    IDLE UMETA(DisplayName = "IDLE"),
    WALK UMETA(DisplayName = "WALK"),
    SPRINT UMETA(DisplayName = "SPRINT"),
    JUMP UMETA(DisplayName = "JUMP"),
    CROUCH UMETA(DisplayName = "CROUCH"),
    PRONING UMETA(DisplayName = "PRONING"),
    AIM UMETA(DisplayName = "AIM"),
    SWIM UMETA(DisplayName = "SWIM"),
    INJURED UMETA(DisplayName = "INJURED"),
    DEAD UMETA(DisplayName = "DEAD"),
    CROUCH_WALK UMETA(DisplayName = "CROUCH_WALKING"),
    PRONING_WALK UMETA(DisplayName = "PRONING_WALKING"),
    AIM_WALK UMETA(DisplayName = "AIM_WALKING"),
    SPRINT_JUMP UMETA(DisplayName = "JUMP_ON_SPRINT"),
    CROUCH_AIM UMETA(DisplayName = "CROUCH_ON_AIM"),
    PRONING_AIM UMETA(DisplayName = "CROUCH_ON_PRONING"),
    TEST UMETA(DisplayName = "TEST")
};

/**
 * 
 */
UCLASS()
class ANIMATIONS_API UPlayer_anim_instance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
        float speed = 0.f;


    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
    float crouch_walk_speed = 0.f;*/

    bool is_weapon_equipped;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_property")
        e_player_animation_state player_state;

public:
    UPlayer_anim_instance();

    // Override function
    virtual void NativeUpdateAnimation(float _delta_seconds) override;
};
