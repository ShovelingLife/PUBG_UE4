#include "Weapon_manager.h"

// Sets default values
AWeapon_manager::AWeapon_manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon_manager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_manager::Tick(float _delta_time)
{
	Super::Tick(_delta_time);
    //Check_if_reloading(_delta_time);
    //Check_continously_shooting(_delta_time);
}
//
//void AWeapon_manager::Shoot()
//{
//    ACore_weapon* tmp_weapon = nullptr;
//    UWorld* p_world = GetWorld();
//    bool          is_first_equipped = false;
//    bool          is_second_equipped = false;
//
//    if (m_is_reloading)
//        return;
//
//    Verify_equipped_weapon(is_first_equipped, is_second_equipped);
//
//    // 첫번째 무기
//    if (is_first_equipped)
//        tmp_weapon = m_first_weapon;
//
//    else
//        tmp_weapon = m_second_weapon;
//
//    if (tmp_weapon)
//    {
//        auto weapon_data = tmp_weapon->weapon_data;
//
//        // 총알 부족
//        if (weapon_data.current_bullet_count == 0)
//        {
//            if (weapon_data.current_ammunition_count == 0)
//                tmp_weapon->Play_sound(e_weapon_sound_type::EMPTY_AMMO);
//
//            else
//                Reload();
//
//            return;
//        }
//        // 사운드 적용 및 총알 1개 차감
//        weapon_data.current_bullet_count--;
//        tmp_weapon->Play_sound(e_weapon_sound_type::BULLET_SOUND);
//
//        // 레이캐스트 적용
//        APlayerCameraManager* camera_manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
//        FVector               begin_pos = tmp_weapon->skeletal_mesh->GetSocketLocation("Muzzle_socket");
//        FVector               forward_vec = camera_manager->GetActorForwardVector() * 500;
//        FHitResult            hit_result;
//        FVector               end_pos = begin_pos + forward_vec;
//        p_world->LineTraceSingleByObjectType(hit_result, begin_pos, end_pos, FCollisionObjectQueryParams(ECC_Pawn));
//        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
//        FRotator bullet_rotation = UKismetMathLibrary::FindLookAtRotation(begin_pos, end_pos);
//        //DrawDebugLine(p_world, begin_pos, end_pos, FColor::Red, true, 5.f, (uint8)0U, 1.f);
//        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, bullet_rotation.ToString());
//        AActor* bullet = nullptr;
//
//        if (tmp_weapon->p_bullet)
//        {
//            // 총알 회전값 수정 요망
//            auto test_rotation = FRotator::MakeFromEuler(FVector(end_pos.X, bullet_rotation.Pitch, bullet_rotation.Yaw));
//            test_rotation.Yaw += 100.f;
//            bullet = p_world->SpawnActor(tmp_weapon->p_bullet->GetClass(), &begin_pos, &bullet_rotation);
//            //GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Cyan, test_rotation.ToString());
//            /*ACore_bullet* core_bullet = Cast<ACore_bullet>(bullet);
//
//            if (core_bullet)
//                core_bullet->mesh->SetRelativeRotation(bullet_rotation);*/
//        }
//        //if(another_character)
//
//        // 파티클 적용
//        tmp_weapon->p_gun_particle->Activate(true);
//    }
//}
//
//void AWeapon_manager::Reload()
//{
//    ACore_weapon* tmp_weapon = nullptr;
//    int           result = 0;
//    bool          is_first_equipped = false;
//    bool          is_second_equipped = false;
//
//    Verify_equipped_weapon(is_first_equipped, is_second_equipped);
//
//    // 첫번째 무기 착용상태
//    if (is_first_equipped)
//        tmp_weapon = m_first_weapon;
//
//    // 두번째 무기 착용 상태
//    if (is_second_equipped)
//        tmp_weapon = m_second_weapon;
//
//    if (tmp_weapon)
//    {
//        auto weapon_data = tmp_weapon->weapon_data;
//
//        // 허용 총알 개수가 똑같을 시
//        if (weapon_data.current_bullet_count == weapon_data.max_bullet_count)
//            return;
//
//        // 중간에 장전
//        else if (weapon_data.current_bullet_count > 0)
//            result = weapon_data.max_bullet_count - weapon_data.current_bullet_count;
//
//        // 전체 총알 소모 후 장전
//        else
//            result = tmp_weapon->weapon_data.max_bullet_count;
//
//        weapon_data.current_ammunition_count -= result;
//        weapon_data.current_bullet_count += result;
//        m_is_reloading = true;
//        tmp_weapon->Play_sound(e_weapon_sound_type::RELOAD_SOUND);
//    }
//}
//
//void AWeapon_manager::Swap()
//{
//    if (Is_first_weapon_equipped())
//        Select_weapon(e_equipped_weapon_type::FIRST);
//
//    // 두번쨰 무기가 있을 시 교체
//    else if (Is_second_weapon_equipped())
//        Select_weapon(e_equipped_weapon_type::SECOND);
//}
//
//void AWeapon_manager::Change_shoot_mode()
//{
//}
//
//void AWeapon_manager::Change_aim_pose(int _type)
//{
//    bool is_first_equipped = false;
//    bool is_second_equipped = false;
//
//    Verify_equipped_weapon(is_first_equipped, is_second_equipped);
//
//    // 에임 상태 > 원래 상태
//    if (_type == 1)
//    {
//        // 첫번째 무기가 부착 되어있을 시
//        if (is_first_equipped)
//            Attach_first_weapon("hand_weapon_sock");
//
//        else
//        {
//            if (is_second_equipped)
//                Attach_second_weapon("hand_weapon_sock");
//        }
//    }
//    // 원래 상태 > 에임 상태
//    else if (_type == 0)
//    {
//        // 첫번째 무기가 부착 되어있을 시
//        if (is_first_equipped)
//            Select_weapon(e_equipped_weapon_type::FIRST);
//
//        else
//        {
//            if (is_second_equipped)
//                Select_weapon(e_equipped_weapon_type::SECOND);
//        }
//    }
//}
//
//void AWeapon_manager::Check_if_reloading(float _delta_time)
//{
//    // 재장전 중
//    if (m_is_reloading)
//    {
//        m_current_reload_time += _delta_time;
//
//        if (m_current_reload_time > mk_reload_time)
//        {
//            m_current_reload_time = 0.f;
//            m_is_reloading = false;
//        }
//    }
//}
//
//void AWeapon_manager::Check_continously_shooting(float _delta_time)
//{
//    if (m_is_shooting)
//    {
//        m_current_shoot_time += _delta_time;
//
//        if (m_current_shoot_time > m_shoot_time)
//        {
//            Shoot();
//            m_current_shoot_time = 0.f;
//            m_is_shooting = m_is_changed_shoot_type;
//        }
//    }
//}
//
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
//
//void AWeapon_manager::Equip_first_weapon()
//{
//    if (Is_first_weapon_equipped())
//        Select_weapon(e_equipped_weapon_type::FIRST);
//}
//
//void AWeapon_manager::Equip_second_weapon()
//{
//    if (Is_second_weapon_equipped())
//        Select_weapon(e_equipped_weapon_type::SECOND);
//}
//
//void AWeapon_manager::Attach_weapons_back()
//{
//}
//
//void AWeapon_manager::Attach_first_weapon(FString _socket_name)
//{
//    if (m_first_weapon)
//        m_first_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *_socket_name);
//}
//
//void AWeapon_manager::Attach_second_weapon(FString _socket_name)
//{
//    if (m_second_weapon)
//        m_second_weapon->skeletal_mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), *_socket_name);
//}
//
//bool AWeapon_manager::Is_first_weapon_equipped()
//{
//    return (m_first_weapon && !m_first_weapon->is_equipped);
//}
//
//bool AWeapon_manager::Is_second_weapon_equipped()
//{
//    return (m_second_weapon && !m_second_weapon->is_equipped);
//}