#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_manager.generated.h"

class ACore_weapon;
class ACore_melee_weapon;
class ACore_throwable_weapon;

enum class e_equipped_weapon_type
{
    FIRST,
    SECOND,
    SWAP,
	PISTOL,
	GRENADE,
	MELEE,
    NONE
};

UCLASS()
class PLAYER_WEAPONS_API AWeapon_manager : public AActor
{
	GENERATED_BODY()

private:
    // ����� ����
    UPROPERTY(VisibleAnywhere, Category = Audio)
        class UAudioComponent* mp_audio;

    const float mk_reload_time = 2.f;
    float       m_current_reload_time = 0.f;
    float       m_shoot_time = 0.25f;
    float       m_current_shoot_time = 0.f;
    bool        m_is_reloading = false;
    bool        m_is_interacting = false;
    bool        m_is_changed_shoot_type = true;
    bool        m_is_shooting = false;

public:
	ACore_weapon*			p_first_gun	 = nullptr;
    ACore_weapon*			p_second_gun = nullptr;
    ACore_weapon*			p_pistol	 = nullptr;
    ACore_melee_weapon*		p_melee		 = nullptr;
    ACore_throwable_weapon* p_throwable  = nullptr;
	bool					is_shooting  = false;

public:	
	// Sets default values for this actor's properties
	AWeapon_manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//public:
//	// �߻�
//	void Shoot();
//
//	// ����
//	void Reload();
//
//	// ��ü
//	void Swap();
//
//	void Change_shoot_mode();
//
//	void Change_aim_pose(int);
//
//	// ���� ������ Ȯ��
//	void Check_if_reloading(float);
//
//	// ���� / ���� Ȯ��
//	void Check_continously_shooting(float);
//
//	void Select_weapon(e_equipped_weapon_type);
//
//	void Equip_first_weapon();
//
//	void Equip_second_weapon();
//
//	// ------- ���� ���� �Լ� -------
//
//	// �ѱ⸦ �� �ڿ��� ����
//	void Attach_weapons_back();
//
//	// ù��° ���⸦ ���Ͽ��� ����
//	void Attach_first_weapon(FString);
//
//	// �ι�° ���⸦ ���Ͽ��� ����
//	void Attach_second_weapon(FString);
//
//	bool Is_first_weapon_equipped();
//
//	bool Is_second_weapon_equipped();
};