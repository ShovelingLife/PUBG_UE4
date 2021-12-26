// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_interaction.generated.h"

UCLASS()
class PUBG_UE4_API ABase_interaction : public AActor
{
	GENERATED_BODY()
	
protected:

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    class UStaticMeshComponent* m_static_mesh = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Collider)
    class UBoxComponent* m_box_collider = nullptr;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)
    class USkeletalMeshComponent* skeletal_mesh = nullptr;

    TSubclassOf< UUserWidget> widget_bp_class;
    class UInteraction_UI*    p_widget;

    UPROPERTY(VisibleAnywhere, Category = UI)
    class UWidgetComponent* p_widget_component;

public:
    // Sets default values for this actor's properties
    ABase_interaction();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void Set_UI_widget_text(FText);

    void Init_static_mesh(FString, FName);

    void Init_skeletal_mesh(FString, FName);

    // Called every frame
    virtual void Tick(float) override;

public:
    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

    // Function which interacts
    virtual void Interact() { };
};