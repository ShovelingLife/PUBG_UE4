/**
 * \file Other_weapon_data.h
 *
 * \brief ��Ÿ ���� ������
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OtherWeaponData.generated.h"

 /**
   * \brief CSV�κ��� ������� ��Ÿ ����(��ô��,����) ���� ����ü
  */
USTRUCT(BlueprintType)
struct FsOtherWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	/** \brief ��Ÿ ������Ʈ ��� ���� */
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString Type		 = "";
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString GroupType	 = "";
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString AudioPath	 = "";
    UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString ParticlePath = "";
    UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FString Description  = "";

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) FVector MeshPos		= FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float MeshRotationX = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float MeshSize		= 0.f;

	/** \brief �浹ü ������Ʈ ���� */
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float ColliderPosZ	 = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float ColliderHeight = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float ColliderSize	 = 0.f;

	/** \brief ���� ���� ���� */
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float Damage		  = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float ExplosionRadius = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data) float ExplosionTime	  = 0.f;
	FString MeshPath = "";

public:
	FsOtherWeaponData() = default;
};