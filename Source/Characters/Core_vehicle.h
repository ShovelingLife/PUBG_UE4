// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Base_interaction_pawn.h"
#include "Core_vehicle.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API ACore_vehicle : public ABase_interaction_pawn
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, Category = Collider)
        class UBoxComponent* mp_interation_collider = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
    class USpringArmComponent* mp_spring_arm = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
    class UCameraComponent* mp_camera = nullptr;

    Fs_vehicle_data m_vehicle_data;
    FString         m_vehicle_type;

public:
    // Sets default values for this actor's properties
    ACore_vehicle();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    void Init(e_vehicle_type);

    void Init_camera();

    void Update_mesh();

    void Init_interaction_collider();
};