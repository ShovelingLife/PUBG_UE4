/**
 * \file Other_weapon_data.h
 *
 * \brief 기타 무기 데이터
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OtherWeaponData.generated.h"

 /** \brief CSV로부터 가지고올 기타 무기(투척류,근접) 정보 구조체 */
USTRUCT(BlueprintType)
struct FsOtherWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	/** \brief 기타 컴포넌트 경로 관련 */
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) FString Type		 = "";
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) FString GroupType	 = "";
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) FString AudioPath	 = "";
    UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) FString ParticlePath = "";
    UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) FString Description  = "";

	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) FVector MeshPos = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) float MeshRotationX = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) float MeshSize		= 0.f;

	/** \brief 무기 정보 관련 */
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) float Damage = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) float ExplosionRadius = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = OtherWeaponData) float ExplosionTime	  = 0.f;
	FString MeshPath = "";

public:
	FsOtherWeaponData() = default;

	//FORCEINLINE FsOtherWeaponData& operator=(const FsOtherWeaponData& Src)
	//{
	//	this->Type		= Src.Type;
	//	this->GroupType = Src.GroupType;
	//	this->AudioPath = Src.AudioPath;
	//	this->ParticlePath = Src.ParticlePath;
	//	this->Description = Src.Description;

	//	// 메시 관련
	//	this->MeshPos = Src.MeshPos;
 //       this->MeshRotationX = Src.MeshRotationX;
 //       this->MeshSize = Src.MeshSize;

 //       this->Damage = Src.Damage;
 //       this->ExplosionRadius = Src.ExplosionRadius;
 //       this->ExplosionTime = Src.ExplosionTime;
	//	return *this;
	//}
};