#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/My_enum.h"
#include "Weapon_enum.h"
#include "Weapon_manager.generated.h"

class ABase_interaction;
class ACore_weapon;
class ACore_melee_weapon;
class ACore_throwable_weapon;

UCLASS()
class PLAYER_WEAPONS_API AWeapon_manager : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Scene_comp)
	class USceneComponent* mp_scene_comp = nullptr;

    const float	mk_reload_time			= 2.f;
    float		m_current_reload_time	= 0.f;
    float		m_shoot_time			= 0.25f;
    float		m_current_shoot_time	= 0.f;
    bool		m_is_reloading		    = false;
    bool		m_is_interacting	    = false;
    bool		m_is_changed_shoot_type = true;
    bool		m_is_shooting			= false;

public:
	ACore_weapon*			p_first_gun			= nullptr;
    ACore_weapon*			p_second_gun		= nullptr;
    ACore_weapon*			p_pistol			= nullptr;
    ACore_melee_weapon*		p_melee				= nullptr;
    ACore_throwable_weapon* p_throwable			= nullptr;
    e_current_weapon_type	current_weapon_type = e_current_weapon_type::NONE;
	bool					arr_is_weapon_equipped[5]{ false };
	bool					is_shooting			= false;

public:	
	// Sets default values for this actor's properties
	AWeapon_manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// ���� ���� ���� üũ
    void Check_for_equipped_weapon();

	// ���� ���� ���� ����
	void Update_current_equipped_weapon();

	// ���� ���
    void Play_sound(e_weapon_sound_type);

	// �迭 ���� ���� ã�� / ���� / ���� ��ġ
    e_current_weapon_type Find_weapon_index(FString, int);

	// ����
    void Attach_weapon(ABase_interaction*, FString);

	// Ż�� �� ��ġ �ʱ�ȭ
    void Reset_weapon_after_detaching(ABase_interaction*, FTransform);

public:
	// ����
	void Equip(AActor*);

	// �߻�
	void Shoot();

	// ����
	void Reload();

	// ���콺 ��
	bool Scroll_select(int);

	// ��ü ���� ���� / �� ���� / ���� �̸�
	void Swap(ABase_interaction*, AActor*, FString);

	void Change_shoot_mode();

	void Change_aim_pose(int);

	// ���� ������ Ȯ��
	void Check_if_reloading(float);

	// ���� / ���� Ȯ��
	void Check_continously_shooting(float);

	// ���� ��ü�� �������� Ȯ��
	bool Check_if_weapon_available(e_current_weapon_type);

	// ------- ���� ���� �Լ� -------

    int Get_weapon_max_bullet_count(e_current_weapon_type);

    // ���� ������ ������
    ACore_weapon* Get_weapon_type(e_current_weapon_type);

	// ------- UI ���� �Լ� -------

	// ���⸦ �ʿ��ٰ� ����
	void Drop(ABase_interaction*);
};