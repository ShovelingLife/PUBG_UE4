#include "Base_interaction_pawn.h"
#include "UI_PUBG/Interaction_UI.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ABase_interaction_pawn::ABase_interaction_pawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_box_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

    // 위젯 컴포넌트 초기화
    p_widget_component = CreateDefaultSubobject<UWidgetComponent>(TEXT("UI"));
    auto bp_class = ConstructorHelpers::FClassFinder<UInteraction_UI>(TEXT("/Game/Blueprints/UI/BP_UI_interact_obj"));
    widget_bp_class = bp_class.Class;

    AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

// Called when the game starts or when spawned
void ABase_interaction_pawn::BeginPlay()
{
    Super::BeginPlay();
    m_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // 위젯 설정
    if (widget_bp_class)
    {
        p_widget_component->SetWidgetClass(widget_bp_class);
        p_widget_component->SetRelativeLocation(FVector::ZeroVector);
        p_widget_component->SetWidgetSpace(EWidgetSpace::Screen);
        p_widget = Cast<UInteraction_UI>(p_widget_component->GetWidget());
    }
}

// Called every frame
void ABase_interaction_pawn::Tick(float _delta_time)
{
    Super::Tick(_delta_time);

    if (!m_move_pos.IsNearlyZero())
    {
        const FVector new_pos = GetActorLocation() + (m_move_pos * _delta_time * m_move_speed);
        SetActorLocation(new_pos);
    }
    // 차량 > 플레이어 전환
    auto current_controller = UGameplayStatics::GetPlayerController(this, 0);

    if (m_is_interacting)
    {
        m_current_pressing_time += current_controller->GetInputKeyTimeDown(EKeys::F);

        if (m_player &&
            m_current_pressing_time > 0.5f)
        {
            m_current_pressing_time = 0.f;
            current_controller->UnPossess();
            current_controller->Possess(Cast< APawn>(m_player));
        }
    }

}

// Called to bind functionality to input
void ABase_interaction_pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // F키
    InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ABase_interaction_pawn::Begin_interact);
    InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Released, this, &ABase_interaction_pawn::End_interact);

    // 이동
    InputComponent->BindAxis(TEXT("Up_down"), this, &ABase_interaction_pawn::Move_up_down);
    InputComponent->BindAxis(TEXT("Left_right"), this, &ABase_interaction_pawn::Move_left_right);
}

void ABase_interaction_pawn::NotifyActorBeginOverlap(AActor* _collided_actor)
{
    Super::NotifyActorBeginOverlap(_collided_actor);

    if (m_player == Cast<ACharacter>(_collided_actor))
        p_widget_component->SetVisibility(true);
}

void ABase_interaction_pawn::NotifyActorEndOverlap(AActor* _collided_actor)
{
    Super::NotifyActorEndOverlap(_collided_actor);
    p_widget_component->SetVisibility(false);
}


void ABase_interaction_pawn::Set_UI_widget_text(FText _text)
{
    p_widget->Title_txt->SetText(_text);
}

void ABase_interaction_pawn::Init_skeletal_mesh(FString _path, FName _name)
{
    // 메시 생성
    skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(_name);
    RootComponent = skeletal_mesh;
    m_box_collider->AttachToComponent(skeletal_mesh, FAttachmentTransformRules::KeepRelativeTransform);

    // 경로로부터 메시 생성
    ConstructorHelpers::FObjectFinder<USkeletalMesh> MESH(*_path);

    if (MESH.Succeeded())
        skeletal_mesh->SetSkeletalMesh(MESH.Object);

    p_widget_component->AttachToComponent(skeletal_mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABase_interaction_pawn::Move_up_down(float _value)
{
    m_move_pos.X = FMath::Clamp(_value, -1.f, 1.f);

}

void ABase_interaction_pawn::Move_left_right(float _value)
{
    m_move_pos.Y = FMath::Clamp(_value, -1.f, 1.f);
}