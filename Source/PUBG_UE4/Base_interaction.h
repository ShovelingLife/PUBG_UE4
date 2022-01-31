#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_interaction.generated.h"

class USceneComponent;

UCLASS()
class PUBG_UE4_API ABase_interaction : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleAnywhere, Category = Collider)
    class UBoxComponent* m_box_collider = nullptr;

    // 상호작용 관련 변수
    UPROPERTY(VisibleAnywhere, Category = Widget)
    class UWidgetComponent* mp_interaction_widget_comp;

    float   m_current_time = 0.f;

public:
    UPROPERTY(VisibleAnywhere, Category = Mesh)
        class UStaticMeshComponent* p_static_mesh_comp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Mesh)
    class USkeletalMeshComponent* p_skeletal_mesh_comp = nullptr;

    UPROPERTY(VisibleAnywhere, Category = Sound)
        class UAudioComponent* p_audio_comp = nullptr;

    FString object_type;
    bool is_player_near = false;

public:
    // Sets default values for this actor's properties
    ABase_interaction();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float) override;

protected:
    void Init_static_mesh(FString, FName);

    void Init_skeletal_mesh(FString, FName);

    void Init_audio();

    void Init_interaction_UI();
};