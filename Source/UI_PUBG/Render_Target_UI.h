// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Render_Target_UI.generated.h"

UCLASS()
class UI_PUBG_API ARender_Target_UI : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = Weapon_UI)
        class USceneCaptureComponent2D* p_scene_capture_component;

    UPROPERTY(EditAnywhere, Category = Weapon_UI)
        class UTextureRenderTarget2D* p_render_target;

    UPROPERTY(EditAnywhere, Category = Weapon_UI_Background)
        class UStaticMeshComponent* p_background_static_mesh;

    UPROPERTY(EditAnywhere, Category = Weapon_UI_Obj)
        class UStaticMeshComponent* p_obj_to_draw_static_mesh;

public:	
	// Sets default values for this actor's properties
	ARender_Target_UI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    // 렌더 타깃 초기화
    void Init_render_target();

    // 메쉬 초기화
    void Init_mesh();
};
