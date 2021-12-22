// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Custom_enum.h"
#include "Core_vehicle.generated.h"
class ACustom_player;

UCLASS()
class CHARACTERS_API ACore_vehicle : public AWheeledVehicle
{
    GENERATED_BODY()

private:
    FVector m_move_pos;
    float   m_move_speed = 500.f;

protected:
    UPROPERTY(VisibleAnywhere, Category = Collider)
        class UBoxComponent* m_box_collider = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Collider)
        class UBoxComponent* mp_interation_collider = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class USpringArmComponent* mp_spring_arm = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Camera)
        class UCameraComponent* mp_camera = nullptr;

    FString               m_mesh_path = "";
    ACustom_player*       m_player;
    Fs_vehicle_data       m_vehicle_data;
    int                   m_current_player_count = 0;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)
        class    USkeletalMeshComponent* skeletal_mesh = nullptr;

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

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    // Called every frame
    virtual void Tick(float) override;

protected:
    void Init(e_vehicle_type);

    void Init_camera();

    void Update_mesh();

    void Init_interaction_collider();

    void Load_from_csv_vehicle(e_vehicle_type, Fs_vehicle_data&);

    void Init_default_components();

    void Init_skeletal_mesh(FString);

    void Player_exit();

    // 
    void Move_up_down(float);

    // 
    void Move_left_right(float);

public:
    // 탑승 가능 여부 확인
    bool Check_if_seat_available(ACustom_player*);
};