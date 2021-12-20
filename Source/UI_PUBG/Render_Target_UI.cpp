#include "Render_Target_UI.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ARender_Target_UI::ARender_Target_UI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    Init_render_target();
	Init_mesh();
}

// Called when the game starts or when spawned
void ARender_Target_UI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARender_Target_UI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARender_Target_UI::Init_render_target()
{
    p_scene_capture_component = CreateDefaultSubobject< USceneCaptureComponent2D >(TEXT("Scene_capture_component"));
    RootComponent = p_scene_capture_component;
    p_scene_capture_component->FOVAngle = 55.f;
}

void ARender_Target_UI::Init_mesh()
{
    // 배경 ui 세부설정
    p_background_static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_Background_UI"));
    p_background_static_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    p_background_static_mesh->SetCastShadow(false);

    p_background_static_mesh->SetRelativeTransform(FTransform(FRotator::MakeFromEuler(FVector(0.f, 270.f, 180.f)), FVector(70.f, 0.f, 0.f), FVector(0.025f)));
    ConstructorHelpers::FObjectFinder<UStaticMesh> UI_BACKGROUND_MESH(TEXT("/Game/UI/Plane"));

    if (UI_BACKGROUND_MESH.Succeeded())
        p_background_static_mesh->SetStaticMesh(UI_BACKGROUND_MESH.Object);

    // 아이콘 ui 세부설정
    p_obj_to_draw_static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_Background_Obj"));
    p_obj_to_draw_static_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    p_obj_to_draw_static_mesh->SetRelativeLocation(FVector(60.f, 0.f, 0.f));
    p_obj_to_draw_static_mesh->SetCastShadow(false);
}