#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Other_weapon_data.generated.h"

// CSV로부터 가지고올 기타 무기(투척류,근접) 정보 구조체
USTRUCT(BlueprintType)
struct Fs_other_weapon_data : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
		FString type = "";

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
		FString weapon_group = "";

    UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
	FString audio_path = "";

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
	FString particle_path = "";

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
	FVector collider_size = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
	FVector collider_pos = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
	float damage = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = Other_weapon_data)
        float radius = 0.f;

    FString mesh_path = "";

public:
	Fs_other_weapon_data() = default;

};

UCLASS()
class PUBG_UE4_API AOther_weapon_data : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOther_weapon_data();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
