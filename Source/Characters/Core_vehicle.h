// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PUBG_UE4/My_enum.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Core_vehicle.generated.h"
class ACustom_player;
class APawn;

UCLASS()
class CHARACTERS_API ACore_vehicle : public AWheeledVehiclePawn
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = Camera)
        class USpringArmComponent* mp_spring_arm = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class UCameraComponent* mp_camera = nullptr;

    FString               m_mesh_path = "";
    ACustom_player* m_player;
    Fs_vehicle_data       m_vehicle_data;
    int                   m_current_player_count = 0;

public:
    // ����
    TSubclassOf< UUserWidget>	widget_bp_class;

    UPROPERTY(VisibleAnywhere, Category = UI)
        class UWidgetComponent* p_widget_component;

public:
    // Sets default values for this actor's properties
    ACore_vehicle();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    virtual void SetupPlayerInputComponent(class UInputComponent*) override;

public:
    // Called every frame
    virtual void Tick(float) override;

protected:
    void Load_from_csv_vehicle(e_vehicle_type, Fs_vehicle_data&);

    void Init(e_vehicle_type);

    void Init_UI();

    void Init_camera();

    void Init_skeletal_mesh(FString);

    void Player_exit();

    // 
    void Accelerate(float);

    void Brake(float);

    // 
    void Handling(float);

public:
    // 탑승 가능 여부 확인
    bool Check_if_seat_available(ACustom_player*);
};