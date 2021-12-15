// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom_enum.generated.h"

UCLASS()
class PUBG_UE4_API ACustom_enum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustom_enum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UENUM(BlueprintType)
enum class e_player_state : uint8
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

enum class e_weapon_sound_type
{
    EMPTY_AMMO,
    BULLET_SOUND,
    RELOAD_SOUND
};

enum class e_weapon_type
{
    AR4,
    KA_Val,
    Ka47,
    KA74U,
    SMG11,
    G67_Grenade
};