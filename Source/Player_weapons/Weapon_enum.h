#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_enum.generated.h"

enum class e_current_weapon_type
{
    FIRST,
    SECOND,
    PISTOL,
    MELEE,
    THROWABLE,
    NONE
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
    FRAGMENTATION1,
    FRAGMENTATION2,
    ILLUMINATION,
    GRAY_SMOKE,
    RED_SMOKE,
    STICK,
    MOLOTOV,
    CLAYMORE,
    MAX
};

// 근접 무기
enum class e_melee_weapon_type : uint8
{
    PAN = (uint8)e_throwable_weapon_type::MAX,
    KNIFE,
    MAX
};

// 총알 격발 종류
enum class e_gun_shoot_type
{
    SINGLE,
    BURST,
    CONSECUTIVE,
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
