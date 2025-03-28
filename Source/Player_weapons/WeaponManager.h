﻿/**
 * \file Weapon_manager.h
 *
 * \brief 플레이어 무기 매니저 관련
 *
 * \ingroup Player_weapons
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "WeaponManager.generated.h"

DECLARE_DELEGATE(FDeleAim)

#pragma region 클래스 전방 선언
class ABaseInteraction;
class ACoreAttachment;
class ACoreWeapon;
class ACoreMeleeWeapon;
class ACoreThrowableWeapon;
class AWeaponManager;

class USceneComponent;
class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;
class UMaterial;
#pragma endregion

UCLASS()
class PLAYER_WEAPONS_API AWeaponManager : public AActor
{
	GENERATED_BODY()

using enum EWeaponType;

private:
    const TArray<FString> mArrSocketName
    {
        "FirstGunSock",
        "SecondGunSock",
        "HandGunSock"
    };

	TArray<ABaseInteraction*> mArrWeapon;

	// 총기 관련
    bool  mbThrowingGrenade = false;

public:
#pragma region 현재 착용 중인 무기

	UPROPERTY() 
    ACoreWeapon* pFirstGun = nullptr;

    UPROPERTY()
	ACoreWeapon* pSecondGun = nullptr;

    UPROPERTY() 
	ACoreWeapon* pPistol = nullptr;

    UPROPERTY() 
	ACoreMeleeWeapon* pMelee = nullptr;

    UPROPERTY() 
	ACoreThrowableWeapon* pThrowable = nullptr;

	FDeleAim DeleAim;

    EWeaponType CurrentWeaponType = NONE;

#pragma endregion

#pragma region 투척류 관련 변수

    UPROPERTY(EditAnywhere, Category = GrenadeVariable) 
	TArray<USplineMeshComponent*> arrSplineMeshComp;

    UPROPERTY(EditAnywhere, Category = GrenadeVariable) 
	TSubclassOf<AActor> BP_GrenadeEndPoint = nullptr;

    UPROPERTY(EditAnywhere, Category = GrenadeVariable) 
	AActor* GrenadeEndPoint = nullptr;

    UPROPERTY(EditAnywhere, Category = GrenadeVariable) 
	USplineComponent* SplineComp = nullptr;

    UPROPERTY(EditAnywhere, Category = GrenadeVariable) 
	UStaticMesh* PathMesh;

    UPROPERTY(EditAnywhere, Category = GrenadeVariable) 
	UMaterial* PathMat;

	TArray<ACoreAttachment*> arrAttachment;

    FVector mGrenadeVelocity;
    float GrenadeDirection;
	float GrenadeSpeed = 0.f;

#pragma endregion

public:	
	AWeaponManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	// 착용 중인 무기를 체크함 미구현상태
    void CheckForEquippedWeapon();

    void InitGrenadePath();

    UFUNCTION() 
	void PredictGrenadePath();

    /**
     * \brief 무기를 플레이어 메시에 부착
	 * \param pNewWeapon 무기 종류
	 * \param SocketName 소켓 이름
	 * \param bCheck 무기 중복 여부 체크
     */
    void AttachWeapon(ABaseInteraction* pNewWeapon, FString SocketName, bool bCheck = true);

	// 무기 버린 후 초기화
    void Detach(ABaseInteraction* pWeapon);

public:
#pragma region 게터/세터

	ABaseInteraction* GetWeaponByIndex(EWeaponType WeaponType) const;

	// 무기의 인덱스를 구함
	EWeaponType GetWeaponIndex(ABaseInteraction* pWeapon) const;

	ACoreWeapon* GetCurrentGun() const;

	// 무기의 종류를 구함
	int GetWeaponType(ABaseInteraction* pWeapon) const;

#pragma endregion

	// 발사
	void ClickEvent();

#pragma region 투척류 관련 함수

	void CreateExplosive(ACoreThrowableWeapon* pGrenade = nullptr);

	void ThrowGrenade();

	void ResetGrenadePath();

	void UpdateGrenadePath();

#pragma endregion

#pragma region 총기 관련 함수
	/**
	 * \brief 마우스 휠 통해 무기 교체
	 * \param Pos 현재 착용 중인 무기의 위치
	 */
	bool ScrollSelect(FString Pos);

	/**
	 * \brief 무기 착용
	 * \param pWeapon 착용할 무기
	 * \param bCheck 무기 중복 여부 체크
	 */
	bool TryToEquip(ABaseInteraction* pWeapon, bool bCheck = true);

	/**
	 * \brief 무기 교체
	 * \param CurrentWeapon 현재 착용 중인 무기
	 * \param NewWeapon 새로운 무기
	 * \param SocketName 메시 소켓 명칭
	 */
	void SwapWorld(ABaseInteraction* pNewWeapon, AActor* pCurrentWeapon, FString SocketName);

	// 현재 착용 중인 무기랑 교체
	bool Swap(EWeaponType WeaponType, bool bScrolling = false);

	// 반환값 = -1 에러 / 0 성공 / 1 근접 또는 투척 무기
	int Swap(ABaseInteraction* pCurrentWeapon, ABaseInteraction* pNewWeapon = nullptr, EWeaponType WeaponType = NONE);

	// 착용 중인 무기를 체크함
	void ChangeShootMode();

#pragma endregion

	// ------- 부착 관련 함수 -------

    /**
     * \brief 현재 착용 중인 무기를 갖고옴
     * \param WeaponType 무기 종류
     * \return 현재 착용 중인 무기
     */

#pragma region UI 관련 함수

	/**
	 * \brief 무기를 맵에다가 버림
	 * \param WeaponType 버릴 무기 종류
	 */
	void Drop(EWeaponType WeaponType);

	void SetNull(EWeaponType WeaponType);

	void SetMeshToPlayerUI(TArray<AActor*> pArrActor);

	void DeactivateFiring();

    bool IsWrongType(ABaseInteraction* pWeapon, EWeaponType WeaponType, bool bFromWeaponSlot);

	bool IsFiring();

#pragma endregion
};