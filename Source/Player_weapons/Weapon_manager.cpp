#include "Weapon_manager.h"
#include "Core_bullet.h"
#include "Core_melee_weapon.h"
#include "Core_throwable_weapon.h"
#include "Core_weapon.h"
#include "PUBG_UE4/Base_interaction.h"
#include "PUBG_UE4/Sound_manager.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"

AWeapon_manager::AWeapon_manager()
{
	PrimaryActorTick.bCanEverTick = true;
    mp_scene_comp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_comp"));
    RootComponent = mp_scene_comp;
}

void AWeapon_manager::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon_manager::Tick(float _delta_time)
{
	Super::Tick(_delta_time);
    Check_if_reloading(_delta_time);
    Check_continously_shooting(_delta_time);
    Update_current_equipped_weapon();
    Check_for_equipped_weapon();
}

void AWeapon_manager::Update_current_equipped_weapon()
{
    arr_is_weapon_equipped[(int)e_current_weapon_type::FIRST]     = (p_first_gun  != nullptr);
    arr_is_weapon_equipped[(int)e_current_weapon_type::SECOND]    = (p_second_gun != nullptr);
    arr_is_weapon_equipped[(int)e_current_weapon_type::PISTOL]    = (p_pistol     != nullptr);
    arr_is_weapon_equipped[(int)e_current_weapon_type::MELEE]     = (p_melee      != nullptr);
    arr_is_weapon_equipped[(int)e_current_weapon_type::THROWABLE] = (p_throwable  != nullptr);
}

void AWeapon_manager::Play_sound(e_weapon_sound_type _sound_type)
{
    // ����� ������Ʈ ��������
    UAudioComponent* p_audio_comp = nullptr;
    int              weapon_index = 0;
    
    // ù��° ����
    if (arr_is_weapon_equipped[0])
    {
        p_audio_comp = p_first_gun->p_audio_comp;
        weapon_index = (int)p_first_gun->weapon_type;
    }
    // �ι�° ����
    if (arr_is_weapon_equipped[1])
    {
        p_audio_comp = p_second_gun->p_audio_comp;
        weapon_index = (int)p_second_gun->weapon_type;
    }
    // ����° ����
    if (arr_is_weapon_equipped[2])
    {
        p_audio_comp = p_pistol->p_audio_comp;
        weapon_index = (int)p_pistol->weapon_type;
    }
    // �׹�° ����
    if (arr_is_weapon_equipped[3])
    {
        p_audio_comp = p_melee->p_audio_comp;
        weapon_index = (int)p_melee->weapon_type;
    }
    // �ټ���° ����
    if (arr_is_weapon_equipped[4])
    {
        p_audio_comp = p_throwable->p_audio_comp;
        weapon_index = (int)p_throwable->weapon_type;
    }
    /*if (_sound_type != e_weapon_sound_type::SHOT)
        AGlobal::Get_sound_manager()->Play_weapon_sound(p_audio_comp, _sound_type);

    else
        AGlobal::Get_sound_manager()->Play_weapon_sound(p_audio_comp, _sound_type, weapon_index);*/
}

e_current_weapon_type AWeapon_manager::Find_weapon_index(FString _direction, int _start_index)
{
    // ������ �Ʒ�
    if (_direction == "down")
    {
        for (int i = _start_index; i > -1; i--)
        {
            if (arr_is_weapon_equipped[i])
                return (e_current_weapon_type)i;
        }
    }
    // �Ʒ����� ��
    else
    {
        for (int i = _start_index; i < 5; i++)
        {
            if (arr_is_weapon_equipped[i])
                return (e_current_weapon_type)i;
        }
    }
    return e_current_weapon_type::NONE;
}

void AWeapon_manager::Equip(AActor* _p_weapon)
{
    ABase_interaction* tmp_collided_weapon = Cast<ABase_interaction>(_p_weapon);

    if (!tmp_collided_weapon)
        return;

    // �ѱ� ����
    if (_p_weapon->IsA<ACore_weapon>())
    {
        // ������ �� ������ 3�� ����
        if (Cast<ACore_weapon>(_p_weapon)->weapon_data.weapon_group_type == "Handgun")
        {
            if (!p_pistol)
                Attach_weapon(tmp_collided_weapon, "hand_gun_sock");

            else
                Swap(p_pistol, _p_weapon, "hand_gun_sock");
        }
        // ��Ÿ �ѱ� 1,2�� ����
        else
        {
            if (!p_first_gun) // ù��° ���Ⱑ ���� ��
                Attach_weapon(tmp_collided_weapon, "first_gun_sock");

            else
            {
                if (!p_second_gun) // �ι�° ���Ⱑ ���� ��
                    Attach_weapon(tmp_collided_weapon, "second_gun_sock");

                else
                {
                    // ù��° ���� ������
                    if      (current_weapon_type == e_current_weapon_type::FIRST)
                             Swap(p_first_gun, _p_weapon, "first_gun_sock");

                    // ��° ���� ������
                    else if (current_weapon_type == e_current_weapon_type::SECOND)
                             Swap(p_second_gun, _p_weapon, "second_gun_sock");
                }
            }
        }
    }
    // ���� ����
    else if (_p_weapon->IsA<ACore_melee_weapon>())
             Attach_weapon(tmp_collided_weapon, "equipped_weapon_pos_sock");

    // ��ô��
    else if (_p_weapon->IsA<ACore_throwable_weapon>())
             Attach_weapon(tmp_collided_weapon, "equipped_weapon_pos_sock");
}

void AWeapon_manager::Shoot()
{
    ACore_weapon* tmp_weapon = Get_weapon_type(current_weapon_type);

    if (m_is_reloading)
        return;

    if (tmp_weapon)
    {
        auto weapon_data = tmp_weapon->weapon_data;

        // �Ѿ� ����
        if (weapon_data.current_bullet_count == 0)
        {
            if (weapon_data.current_bullet_count == 0)
                Play_sound(e_weapon_sound_type::EMPTY_AMMO);

            else
                Reload();

            return;
        }
        // ���� ���� �� �Ѿ� 1�� ����
        weapon_data.current_bullet_count--;
        Play_sound(e_weapon_sound_type::SHOT);

        // ����ĳ��Ʈ ����
        APlayerCameraManager* camera_manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FVector               begin_pos = tmp_weapon->p_skeletal_mesh_comp->GetSocketLocation("Muzzle_socket");
        FVector               forward_vec = camera_manager->GetActorForwardVector() * 500;
        FHitResult            hit_result;
        FVector               end_pos = begin_pos + forward_vec;
        GetWorld()->LineTraceSingleByObjectType(hit_result, begin_pos, end_pos, FCollisionObjectQueryParams(ECC_Pawn));
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        FRotator bullet_rotation = UKismetMathLibrary::FindLookAtRotation(begin_pos, end_pos);
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, bullet_rotation.ToString());
        AActor* bullet = nullptr;

        if (tmp_weapon->p_bullet)
        {
            // �Ѿ� ȸ���� ���� ���
            auto test_rotation = FRotator::MakeFromEuler(FVector(end_pos.X, bullet_rotation.Pitch, bullet_rotation.Yaw));
            test_rotation.Yaw += 100.f;
            bullet = GetWorld()->SpawnActor(tmp_weapon->p_bullet->GetClass(), &begin_pos, &bullet_rotation);
            //GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Cyan, test_rotation.ToString());
            /*ACore_bullet* core_bullet = Cast<ACore_bullet>(bullet);

            if (core_bullet)
                core_bullet->mesh->SetRelativeRotation(bullet_rotation);*/
        }
        //if(another_character)

        // ��ƼŬ ����
        tmp_weapon->p_gun_particle->Activate(true);
    }
}

void AWeapon_manager::Reload()
{
    ACore_weapon* tmp_weapon = Get_weapon_type(current_weapon_type);
    int           result     = 0;

    if (tmp_weapon)
    {
        m_is_reloading   = true;
        auto weapon_data = tmp_weapon->weapon_data;

        // ��� �Ѿ� ������ �Ȱ��� ��
        if      (weapon_data.current_bullet_count == weapon_data.max_bullet_count)
                 return;

        // �߰��� ����
        else if (weapon_data.current_bullet_count > 0)
                 result = weapon_data.max_bullet_count - weapon_data.current_bullet_count;

        // ��ü �Ѿ� �Ҹ� �� ����
        else
                 result = tmp_weapon->weapon_data.max_bullet_count;

        weapon_data.current_max_bullet_count -= result;
        weapon_data.current_bullet_count     += result;
        Play_sound(e_weapon_sound_type::RELOAD);
    }
}

bool AWeapon_manager::Scroll_select(int _pos)
{
    // ���� ���� �ε��� ����ͼ� ����
    e_current_weapon_type final_index   = e_current_weapon_type::NONE;
    int                   current_index = (int)current_weapon_type;
    int                   total_weapon  = -1;

    for (int i = 0; i < 5; i++)
    {
        // ���� ���� ���� ����
        if (arr_is_weapon_equipped[i])
            total_weapon++;
    }
    // ���Ⱑ 1���� �� �ű� �ʿ䰡 ����
    if (total_weapon == 0)
        return false;

    // �Ʒ��� ��ũ��
    if (_pos == -1)
    {
        // ������ ���� 0 ���� ��
        if (current_index == (int)e_current_weapon_type::FIRST)
            current_weapon_type = Find_weapon_index("down", (int)e_current_weapon_type::THROWABLE);

        else
        {
            // ���� ��ġ���� Ž��
            final_index = Find_weapon_index("down", current_index - 1);

            // �߰����� ����
            if (final_index == e_current_weapon_type::NONE)
                current_weapon_type = Find_weapon_index("down", (int)e_current_weapon_type::THROWABLE);

            else
                current_weapon_type = final_index;
        }
    }
    // ���� ��ũ��
    else
    {
        // ���� ������ ���ҿ� ���� �� ��
        if (current_index == (int)e_current_weapon_type::THROWABLE)
        {
            if (p_first_gun)
                current_weapon_type = e_current_weapon_type::FIRST;

            else
                current_weapon_type = Find_weapon_index("up", (int)e_current_weapon_type::FIRST);
        }
        else
        {
            // ���� ��ġ���� Ž��
            final_index = Find_weapon_index("up", current_index + 1);

            // �߰����� ����
            if (final_index == e_current_weapon_type::NONE)
                current_weapon_type = Find_weapon_index("up", (int)e_current_weapon_type::FIRST);

            else
                current_weapon_type = final_index;
        }
    }
    return true;
}

void AWeapon_manager::Swap(ABase_interaction* _current_weapon, AActor* _new_weapon, FString _socket_name)
{
    Reset_weapon_after_detaching(_current_weapon, _new_weapon->GetActorTransform());
    Attach_weapon(Cast<ABase_interaction>(_new_weapon), _socket_name);
}

void AWeapon_manager::Change_shoot_mode()
{
    auto p_weapon           = Get_weapon_type(current_weapon_type);
    int  max_gun_shoot_type = 0;

    if (p_weapon)
    {
        FString weapon_group = p_weapon->weapon_data.weapon_group_type;

        // �ܹ�/����/���� ����
        if      (weapon_group == "Assault" ||
                 weapon_group == "SMG")
                 max_gun_shoot_type = (int)e_gun_shoot_type::CONSECUTIVE;

        // �ܹ�/���� ����
        else if (weapon_group == "Shotgun")
                 max_gun_shoot_type = (int)e_gun_shoot_type::BURST;

        // ���� �ܹ߸� ����
        else if (weapon_group == "Handgun" ||
                 weapon_group == "Sniper" ||
                 weapon_group == "Special")
                 max_gun_shoot_type = (int)e_gun_shoot_type::SINGLE;

        // �ݹ� ��� ����
        int current_gun_shoot_type = (int)p_weapon->gun_shoot_type;

        if      ((int)p_weapon->gun_shoot_type < max_gun_shoot_type)
                 p_weapon->gun_shoot_type = (e_gun_shoot_type)++current_gun_shoot_type;

        // �ʱ�ȭ
        else if ((int)p_weapon->gun_shoot_type == max_gun_shoot_type)
                 p_weapon->gun_shoot_type = e_gun_shoot_type::SINGLE;
    }
}

void AWeapon_manager::Change_aim_pose(int _type)
{
    
}

void AWeapon_manager::Check_if_reloading(float _delta_time)
{
    // ������ ��
    if (m_is_reloading)
    {
        m_current_reload_time += _delta_time;

        if (m_current_reload_time > mk_reload_time)
        {
            m_current_reload_time = 0.f;
            m_is_reloading        = false;
        }
    }
}

void AWeapon_manager::Check_continously_shooting(float _delta_time)
{
    if (m_is_shooting)
    {
        m_current_shoot_time += _delta_time;

        if (m_current_shoot_time > m_shoot_time)
        {
            Shoot();
            m_current_shoot_time = 0.f;
            m_is_shooting        = m_is_changed_shoot_type;
        }
    }
}

bool AWeapon_manager::Check_if_weapon_available(e_current_weapon_type _weapon_type)
{
    bool can_switch = false;

    switch (_weapon_type)
    {
    case e_current_weapon_type::FIRST:     if (p_first_gun)  can_switch = true; break;
    case e_current_weapon_type::SECOND:    if (p_second_gun) can_switch = true; break;
    case e_current_weapon_type::PISTOL:    if (p_pistol)     can_switch = true; break;
    case e_current_weapon_type::MELEE:     if (p_melee)      can_switch = true; break;
    case e_current_weapon_type::THROWABLE: if (p_throwable)  can_switch = true; break;
    }
    // ���� �����ϴٸ� ����
    if (can_switch)
        current_weapon_type = _weapon_type;

    return can_switch;
}

void AWeapon_manager::Check_for_equipped_weapon()
{
    
}

int AWeapon_manager::Get_weapon_max_bullet_count(e_current_weapon_type _weapon_type)
{
    ACore_weapon* tmp_weapon = Get_weapon_type(_weapon_type);

    if (tmp_weapon)
        return tmp_weapon->weapon_data.current_max_bullet_count;

    else
        return 1;
}

void AWeapon_manager::Attach_weapon(ABase_interaction* _p_tmp_weapon, FString _socket_name)
{
    auto current_player_mesh  = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh();

    // ���� ��� ���� ���� �Ǻ� �� �ٿ�ĳ����
    if      (_socket_name == "hand_gun_sock")
    {
        p_pistol = Cast<ACore_weapon>(_p_tmp_weapon);
        current_weapon_type = e_current_weapon_type::PISTOL;
    }
    else if (_socket_name == "first_gun_sock")
    {
        p_first_gun = Cast<ACore_weapon>(_p_tmp_weapon);
        current_weapon_type = e_current_weapon_type::FIRST;
    }
    else if (_socket_name == "second_gun_sock")
    {
        // �ߺ� ���� ����
        if (p_first_gun != Cast<ACore_weapon>(_p_tmp_weapon))
        {
            p_second_gun = Cast<ACore_weapon>(_p_tmp_weapon);
            current_weapon_type = e_current_weapon_type::SECOND;
        }
    }
    // ��ô��/��������
    else
    {
        if(_p_tmp_weapon->IsA<ACore_throwable_weapon>())
        {
            p_throwable = Cast<ACore_throwable_weapon>(_p_tmp_weapon);
            current_weapon_type = e_current_weapon_type::THROWABLE;
        }
        else if (_p_tmp_weapon->IsA<ACore_melee_weapon>())
        {
            p_melee = Cast<ACore_melee_weapon>(_p_tmp_weapon);
            current_weapon_type = e_current_weapon_type::MELEE;
        }
    }
    USkeletalMeshComponent* p_skeletal_mesh_comp = _p_tmp_weapon->p_skeletal_mesh_comp;
    UStaticMeshComponent*   p_static_mesh_comp   = _p_tmp_weapon->p_static_mesh_comp;

    // ���� �޽ø� �÷��̾� �޽ÿ� ����
    if      (p_skeletal_mesh_comp)
             p_skeletal_mesh_comp->AttachToComponent(current_player_mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *_socket_name);

    else if (p_static_mesh_comp)
             p_static_mesh_comp->AttachToComponent(current_player_mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *_socket_name);

    // ���� ������Ʈ�� �κ��丮 �Ŵ����� ����
    _p_tmp_weapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AWeapon_manager::Reset_weapon_after_detaching(ABase_interaction* _current_weapon, FTransform _new_pos)
{
    USkeletalMeshComponent* p_skeletal_mesh_comp = _current_weapon->p_skeletal_mesh_comp;
    UStaticMeshComponent*   p_static_mesh_comp   = _current_weapon->p_static_mesh_comp;

    // ������Ʈ�� Ż�� > ���� ��Ʈ ������Ʈ�� ���� > Ʈ������ �ʱ�ȭ
    if (p_skeletal_mesh_comp)
    {
        p_skeletal_mesh_comp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        p_skeletal_mesh_comp->AttachToComponent(_current_weapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        p_skeletal_mesh_comp->ResetRelativeTransform();
    }
    else if (p_static_mesh_comp)
    {
        p_static_mesh_comp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        p_static_mesh_comp->AttachToComponent(_current_weapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        p_static_mesh_comp->ResetRelativeTransform();
    }
    // ���� ���⸦ Ż�� �� ���忡 ��ȯ
    _current_weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    _current_weapon->SetActorTransform(_new_pos);
}

ACore_weapon* AWeapon_manager::Get_weapon_type(e_current_weapon_type _weapon_type)
{
    ACore_weapon* tmp_weapon = nullptr;

    switch (_weapon_type)
    {
    case e_current_weapon_type::FIRST:  tmp_weapon = p_first_gun;  break;
    case e_current_weapon_type::SECOND: tmp_weapon = p_second_gun; break;
    case e_current_weapon_type::PISTOL: tmp_weapon = p_pistol;     break;
    }
    return tmp_weapon;
}

void AWeapon_manager::Drop(ABase_interaction* _p_weapon)
{
    if (!_p_weapon)
        return;

    FVector  final_location  = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
    FRotator player_rotation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorRotation();
    final_location.Y        += 75.f;
    final_location.Z         = 20.f;
    FTransform new_location  = FTransform(player_rotation, final_location);
    Reset_weapon_after_detaching(_p_weapon, new_location);

    // ���� ���� ���� ����� ��
    if      (_p_weapon == p_first_gun)
             p_first_gun = nullptr;

    else if (_p_weapon == p_second_gun)
             p_second_gun = nullptr;

    else if (_p_weapon == p_pistol)
             p_pistol = nullptr;

    else if (_p_weapon == p_melee)
             p_melee = nullptr;

    else if (_p_weapon == p_throwable)
             p_throwable = nullptr;
}
