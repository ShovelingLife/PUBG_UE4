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

    // 상호작용 관련 변수
    UPROPERTY(VisibleAnywhere, Category = Widget_component)
    class UWidgetComponent* mp_interaction_widget_comp;
    FString                 m_object_type;

public:

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    class USkeletalMeshComponent* skeletal_mesh = nullptr;

    bool is_player_near = false;

public:
    // Sets default values for this actor's properties
    ABase_interaction();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float) override;

public:
    virtual void NotifyActorBeginOverlap(AActor*) override;

    virtual void NotifyActorEndOverlap(AActor*) override;

protected:
    void Init_static_mesh(FString, FName);

    void Init_skeletal_mesh(FString, FName);

    void Init_interaction_UI();
};