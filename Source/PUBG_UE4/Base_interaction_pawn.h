// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Base_interaction_pawn.generated.h"

UCLASS()
class PUBG_UE4_API ABase_interaction_pawn : public APawn
{
	GENERATED_BODY()

private:
    FVector m_move_pos;
    float m_move_speed = 500.f;
    float m_current_pressing_time = 0.f;
    bool m_is_interacting = false;

protected:
    UPROPERTY(VisibleAnywhere, Category = Collider)
        class UBoxComponent* m_box_collider = nullptr;

    const FString				mk_vehicle_mesh_path = "/Game/Characters/Vehicle_variaty_pack/Skeletons/SK_";

    class ACharacter* m_player;
public:

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    class    USkeletalMeshComponent* skeletal_mesh = nullptr;

    // À§Á¬
    TSubclassOf< UUserWidget>	widget_bp_class;
    class UInteraction_UI* p_widget;

    UPROPERTY(VisibleAnywhere, Category = UI)
        class UWidgetComponent* p_widget_component;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Sets default values for this pawn's properties
    ABase_interaction_pawn();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    void Set_UI_widget_text(FText);

    void Init_skeletal_mesh(FString, FName);


    void Begin_interact() { m_is_interacting = true; }

    void End_interact() {
        m_is_interacting = false;
        m_current_pressing_time = 0.f;
    }

    // 
    void Move_up_down(float);

    // 
    void Move_left_right(float);
};
