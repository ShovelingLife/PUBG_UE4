// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_enum.generated.h"

enum class e_weapon_sound_type
{
    EMPTY_AMMO,
    BULLET_SOUND,
    RELOAD_SOUND,
    EQUIP_SOUND
};

// 부속품
enum class e_attachment_type : uint8
{

};

// 총기류
enum class e_weapon_type : uint8
{
    C7,
    LS80,
    M1,
    Karos,
    RogerLR220,
    Double_barrel,
    SK12,
    EON,
    PX_70,
    Ayakashi,
    VSS,
    RPG_17,
    Crossbow,
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

UCLASS()
class PLAYER_WEAPONS_API AWeapon_enum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_enum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
