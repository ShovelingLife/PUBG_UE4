#include "Weapon_manager.h"
#include "Core_bullet.h"
#include "Core_melee_weapon.h"
#include "Core_throwable_weapon.h"
#include "Core_weapon.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/Sound_manager.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
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
}

void AWeapon_manager::Update_current_equipped_weapon()
{
    is_first_equipped  = false;
    is_second_equipped = false;
    is_third_equipped  = false;
    is_fourth_equipped = false;
    is_fifth_equipped  = false;

    if (current_weapon_type == e_current_weapon_type::FIRST &&
        p_first_gun)
        is_first_equipped = true;

    if (current_weapon_type == e_current_weapon_type::SECOND &&
        p_second_gun)
        is_second_equipped = true;

    if (current_weapon_type == e_current_weapon_type::PISTOL &&
        p_pistol)
        is_third_equipped = true;

    if (current_weapon_type == e_current_weapon_type::MELEE &&
        p_melee)
        is_fourth_equipped = true;

    if (current_weapon_type == e_current_weapon_type::THROWABLE &&
        p_throwable)
        is_fifth_equipped = true;
}

void AWeapon_manager::Play_sound(e_weapon_sound_type _weapon_sound_type)
{
    // 오디오 컴포넌트 가져오기
    UAudioComponent* p_audio_comp = nullptr;
    int              weapon_index = 0;
    
    // 첫번째 무기
    if (is_first_equipped)
    {
        p_audio_comp = p_first_gun->p_audio_comp;
        weapon_index = (int)p_first_gun->weapon_type;
    }
    // 두번째 무기
    if (is_second_equipped)
    {
        p_audio_comp = p_second_gun->p_audio_comp;
        weapon_index = (int)p_second_gun->weapon_type;
    }
    // 세번째 무기
    if (is_third_equipped)
    {
        p_audio_comp = p_pistol->p_audio_comp;
        weapon_index = (int)p_pistol->weapon_type;
    }
    //// 네번째 무기
    //if (is_fourth_equipped)
    //    p_audio_comp = p_first_gun->p_audio_comp;

    //// 다섯번째 무기
    //if (is_fifth_equipped)
    //    p_audio_comp = p_first_gun->p_audio_comp;

    if (_weapon_sound_type != e_weapon_sound_type::SHOT_SOUND)
        AGlobal::Get_sound_manager()->Play_weapon_sound(p_audio_comp, _weapon_sound_type);

    else
        AGlobal::Get_sound_manager()->Play_weapon_sound(p_audio_comp, _weapon_sound_type, weapon_index);
}

void AWeapon_manager::Equip(AActor* _p_weapon)
{
    // 총기 종류
    if (_p_weapon->IsA<ACore_weapon>())
    {
        ACore_weapon* p_collided_gun = Cast<ACore_weapon>(_p_weapon);

        // 권총일 시 무조건 3번 슬롯
        if (p_collided_gun->weapon_data.weapon_group_type == "Handgun")
        {
            if (!p_pistol)
            {
                p_pistol = p_collided_gun;
                current_weapon_type = e_current_weapon_type::PISTOL;
                Attach_weapon(p_pistol, "hand_gun_sock");
            }
            else
                Swap(p_pistol, _p_weapon, "hand_gun_sock");
        }
        // 기타 총기 1,2번 슬롯
        else
        {
            if (!p_first_gun) // 첫번째 무기가 없을 시
            {
                p_first_gun = p_collided_gun;
                current_weapon_type = e_current_weapon_type::FIRST;
                Attach_weapon(p_first_gun, "first_gun_sock");
            }
            else
            {
                if (!p_second_gun) // 두번째 무기가 없을 시
                {
                    p_second_gun = p_collided_gun;
                    current_weapon_type = e_current_weapon_type::SECOND;
                    Attach_weapon(p_second_gun, "second_gun_sock");
                }
                else
                {
                    // 첫번째 무기 장착중
                    if (current_weapon_type == e_current_weapon_type::FIRST)
                    {
                        current_weapon_type = e_current_weapon_type::FIRST;
                        Swap(p_first_gun, _p_weapon, "first_gun_sock");
                    }
                    // 두째 무기 장착중
                    else if (current_weapon_type == e_current_weapon_type::SECOND)
                    {
                        current_weapon_type = e_current_weapon_type::SECOND;
                        Swap(p_second_gun, _p_weapon, "second_gun_sock");
                    }
                }
            }
        }
    }
    // 근접 종류
    if (_p_weapon->IsA<ACore_melee_weapon>())
    {
        ACore_melee_weapon* p_collided_melee = Cast<ACore_melee_weapon>(_p_weapon);
    }
    // 투척류
    if (_p_weapon->IsA<ACore_throwable_weapon>())
    {
        ACore_throwable_weapon* p_collided_throwable = Cast<ACore_throwable_weapon>(_p_weapon);

    }
}

void AWeapon_manager::Shoot()
{
    ACore_weapon* tmp_weapon = nullptr;
    UWorld*       p_world = GetWorld();

    if (m_is_reloading)
        return;

    switch (current_weapon_type)
    {
    case e_current_weapon_type::FIRST:  tmp_weapon = p_first_gun;  break;
    case e_current_weapon_type::SECOND: tmp_weapon = p_second_gun; break;
    case e_current_weapon_type::PISTOL: tmp_weapon = p_pistol;     break;
    }
    if (tmp_weapon)
    {
        auto weapon_data = tmp_weapon->weapon_data;

        // 총알 부족
        if (weapon_data.current_bullet_count == 0)
        {
            if (weapon_data.current_bullet_count == 0)
                Play_sound(e_weapon_sound_type::EMPTY_AMMO);

            else
                Reload();

            return;
        }
        // 사운드 적용 및 총알 1개 차감
        weapon_data.current_bullet_count--;
        Play_sound(e_weapon_sound_type::SHOT_SOUND);

        // 레이캐스트 적용
        APlayerCameraManager* camera_manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        FVector               begin_pos = tmp_weapon->skeletal_mesh->GetSocketLocation("Muzzle_socket");
        FVector               forward_vec = camera_manager->GetActorForwardVector() * 500;
        FHitResult            hit_result;
        FVector               end_pos = begin_pos + forward_vec;
        p_world->LineTraceSingleByObjectType(hit_result, begin_pos, end_pos, FCollisionObjectQueryParams(ECC_Pawn));
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        FRotator bullet_rotation = UKismetMathLibrary::FindLookAtRotation(begin_pos, end_pos);
        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, bullet_rotation.ToString());
        AActor* bullet = nullptr;

        if (tmp_weapon->p_bullet)
        {
            // 총알 회전값 수정 요망
            auto test_rotation = FRotator::MakeFromEuler(FVector(end_pos.X, bullet_rotation.Pitch, bullet_rotation.Yaw));
            test_rotation.Yaw += 100.f;
            bullet = p_world->SpawnActor(tmp_weapon->p_bullet->GetClass(), &begin_pos, &bullet_rotation);
            //GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Cyan, test_rotation.ToString());
            /*ACore_bullet* core_bullet = Cast<ACore_bullet>(bullet);

            if (core_bullet)
                core_bullet->mesh->SetRelativeRotation(bullet_rotation);*/
        }
        //if(another_character)

        // 파티클 적용
        tmp_weapon->p_gun_particle->Activate(true);
    }
}

void AWeapon_manager::Reload()
{
    ACore_weapon* tmp_weapon = nullptr;
    int           result = 0;

    switch (current_weapon_type)
    {
    case e_current_weapon_type::FIRST:  tmp_weapon = p_first_gun;  break;
    case e_current_weapon_type::SECOND: tmp_weapon = p_second_gun; break;
    case e_current_weapon_type::PISTOL: tmp_weapon = p_pistol;     break;
    }
    if (tmp_weapon)
    {
        m_is_reloading   = true;
        auto weapon_data = tmp_weapon->weapon_data;

        // 허용 총알 개수가 똑같을 시
        if      (weapon_data.current_bullet_count == weapon_data.max_bullet_count)
                 return;

        // 중간에 장전
        else if (weapon_data.current_bullet_count > 0)
                 result = weapon_data.max_bullet_count - weapon_data.current_bullet_count;

        // 전체 총알 소모 후 장전
        else
                 result = tmp_weapon->weapon_data.max_bullet_count;

        weapon_data.current_max_bullet_count -= result;
        weapon_data.current_bullet_count     += result;
        Play_sound(e_weapon_sound_type::RELOAD_SOUND);
    }
}

void AWeapon_manager::Swap(AActor* _current_weapon, AActor* _new_weapon, FString _socket_name)
{
    FVector tmp_location = _current_weapon->GetActorLocation();

    // 현재 무기를 탈착 후 월드에 소환
    //_current_weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    _current_weapon->DetachRootComponentFromParent();
    _current_weapon->SetActorLocation(_new_weapon->GetActorLocation());

    // 맵에 있던 무기를 부착
    Attach_weapon(_new_weapon, _socket_name);
}

void AWeapon_manager::Change_shoot_mode()
{
}

void AWeapon_manager::Change_aim_pose(int _type)
{
    
}

void AWeapon_manager::Check_if_reloading(float _delta_time)
{
    // 재장전 중
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

//void AWeapon_manager::Select_weapon(e_equipped_weapon_type _type)
//{
//    ACore_weapon* tmp_weapon = nullptr;
//
//    switch (_type)
//    {
//    case e_equipped_weapon_type::FIRST: // 첫번째 무기 착용
//
//        if (!m_first_weapon)
//            return;
//
//        if (m_second_weapon)
//            m_second_weapon->is_equipped = false;
//
//        tmp_weapon = m_first_weapon;
//        m_first_weapon->is_equipped = true;
//        break;
//
//    case e_equipped_weapon_type::SECOND: // 두번째 무기 착용
//
//        if (!m_second_weapon)
//            return;
//
//        tmp_weapon = m_second_weapon;
//        m_first_weapon->is_equipped = false;
//        m_second_weapon->is_equipped = true;
//        break;
//
//    case e_equipped_weapon_type::SWAP: // 교체
//
//        if (!m_second_weapon)
//            return;
//
//        tmp_weapon = m_collided_weapon;
//
//        // 맵에 배치된 무기랑 교체
//        m_second_weapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
//        m_second_weapon->SetActorTransform(m_second_weapon->GetTransform());
//        m_second_weapon = nullptr;
//        break;
//
//    case e_equipped_weapon_type::NONE: break;
//    }
//    // 장착
//    if (tmp_weapon)
//    {
//        tmp_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("front_weapon_sock"));
//        tmp_weapon->is_player_near = false;
//        tmp_weapon->Play_sound(e_weapon_sound_type::EQUIP_SOUND);
//    }
//    m_collided_weapon = nullptr;
//}

void AWeapon_manager::Attach_weapons_back()
{
}

void AWeapon_manager::Attach_weapon(AActor* _p_weapon, FString _socket_name)
{
    USkeletalMeshComponent* skeletal_mesh = nullptr;

    if (_p_weapon->IsA<ACore_weapon>())
        skeletal_mesh = Cast<ACore_weapon>(_p_weapon)->skeletal_mesh;



    if (skeletal_mesh)
    {
        skeletal_mesh->AttachToComponent(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *_socket_name);
        _p_weapon->GetRootComponent()->AttachTo(RootComponent);
    }
}

int AWeapon_manager::Get_weapon_max_bullet_count(e_current_weapon_type _weapon_type)
{
    ACore_weapon* tmp_weapon = nullptr;

    switch (_weapon_type)
    {
    case e_current_weapon_type::FIRST:  tmp_weapon = p_first_gun;  break;
    case e_current_weapon_type::SECOND: tmp_weapon = p_second_gun; break;
    case e_current_weapon_type::PISTOL: tmp_weapon = p_pistol;     break;
    }
    if (tmp_weapon)
        return tmp_weapon->weapon_data.current_max_bullet_count;

    else
        return 1;
}