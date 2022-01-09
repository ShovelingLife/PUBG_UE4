// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "My_enum.generated.h"

enum class e_player_state
{
    IDLE,
    WALK,
    SPRINT,
    JUMP,
    CROUCH,
    PRONING,
    AIM,
    THROW,
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

// 총기류
enum class e_weapon_type : uint8
{
    AR4,
    KA_Val,
    Ka47,
    KA74U,
    SMG11,
    MAX
};

// 던지는 무기
enum class e_throwable_weapon_type : uint8
{
    FRAGMENTATION1 = (uint8)e_weapon_type::MAX,
    FRAGMENTATION2,
    ILLUMINATION,
    SMOKE_RED,
    SMOKE_GRAY,
    STICK,
    MAX
};

// 근접 무기
enum class e_melee_weapon_type : uint8
{
    KNIFE = (uint8)e_throwable_weapon_type::MAX,
    PAN,
    MAX
};

// 섭취 아이템
enum class e_consumable_type : uint8
{

};

// 방어구
enum class e_equipment_type : uint8
{

};

// 부속품
enum class e_attachment_type : uint8
{

};

enum class e_vehicle_type
{
    Hatchback,
    Pickup,
    SportsCar,
    SUV,
    Truck,
    Truck_Chassis,
    MAX
};

UENUM()
enum class e_seat_type
{
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    NONE
};

UCLASS()
class PUBG_UE4_API AMy_enum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMy_enum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};