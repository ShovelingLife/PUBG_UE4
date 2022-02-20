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
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "OtherWeaponData.generated.h"

/**
  * \brief CSV로부터 가지고올 기타 무기(투척류,근접) 정보 구조체
 */
USTRUCT(BlueprintType)
struct FsOtherWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	/**
	  * \brief 기타 컴포넌트 경로 관련
	 */
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString Type		 = "";
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString GroupType	 = "";
    UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString AudioPath	 = "";
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString ParticlePath = "";

	/**
	  * \brief 충돌체 컴포넌트 관련
	 */
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FVector ColliderSize = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FVector ColliderPos  = FVector::ZeroVector;

	/**
	  * \brief 무기 정보 관련
	 */
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float Damage = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float Radius = 0.f;
    FString MeshPath = "";

public:
	FsOtherWeaponData() = default;
};

UCLASS()
class PUBG_UE4_API AOther_weapon_data : public AActor
{
	GENERATED_BODY()	
};