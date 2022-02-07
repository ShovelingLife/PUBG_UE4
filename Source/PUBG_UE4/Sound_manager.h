#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data_table_manager.h"
#include "My_enum.h"
#include "Sound_manager.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class PUBG_UE4_API ASound_manager : public AActor
{
	GENERATED_BODY()

private:
    const FString     mk_sound_path = "/Game/SFX/Gun_sounds/";

public:
    // ------- 무기 관련 사운드 -------
    UPROPERTY(VisibleAnywhere, Category = Weapon_sound) TArray<USoundBase*> p_arr_shot_sound;
    UPROPERTY(VisibleAnywhere, Category = Weapon_sound) USoundBase*         p_empty_ammo_sound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase*         p_reload_sound = nullptr;


    // ------- 플레이어 무기 사운드 관련 -------
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* p_item_farm_sound = nullptr; 
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* p_weapon_pickup_sound = nullptr;
    UPROPERTY(VisibleAnywhere, Category = Player_sound) USoundBase* p_weapon_swap_sound = nullptr;

private:
    void Init_weapon_audio();

    void Init_player_audio();

public:	
	// Sets default values for this actor's properties
	ASound_manager();

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
    // 컴포넌트 / 사운드 파일 / 무기 종류
    void Play_weapon_sound(UAudioComponent*, e_weapon_sound_type, int _weapon_index = 0);

    void Play_player_sound(UAudioComponent*, e_player_sound_type);
};