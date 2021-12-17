// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom_enum.generated.h"

UENUM(BlueprintType)
enum class e_player_state : uint8
{
    IDLE,
    WALK,
    SPRINT,
    JUMP,
    CROUCH,
    PRONING,
    AIM,
    SWIM,
    INJURED,
    DEAD,
    CROUCH_WALK,
    PRONING_WALK,
    AIM_WALK,
    SPRINT_JUMP,
    CROUCH_AIM,
    PRONING_AIM,
    TEST
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

enum class e_vehicle_type
{
    Hatchback,
    Pickup,
    SportsCar,
    SUV,
    Truck,
    Truck_Chassis
};

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