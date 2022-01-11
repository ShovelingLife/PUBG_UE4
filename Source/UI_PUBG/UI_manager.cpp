#include "UI_manager.h"
#include "PUBG_UE4/Global.h"
#include "Interaction_UI.h"
#include "Inventory_manager.h"
#include "Player_UI.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AUI_manager::AUI_manager()
{
    PrimaryActorTick.bCanEverTick = true;
    Init_player_UI();
    Init_interaction_UI();
}

// Called when the game starts or when spawned
void AUI_manager::BeginPlay()
{
    Super::BeginPlay();
    mp_global = AGlobal::Get();
    mp_global->dele_update_interaction_widget_comp.BindUFunction(this, "Update_interaction_UI");
    Set_player_UI();
    Set_player_inventory();
}

// Called every frame
void AUI_manager::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
}

void AUI_manager::Init_player_UI()
{
    auto BP_player_UI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_UI_Player.BP_UI_Player_C'"));

    if (BP_player_UI.Succeeded())
        m_bp_player_UI = BP_player_UI.Class;
}

void AUI_manager::Set_player_UI()
{
    UUserWidget* p_widget = CreateWidget(GetWorld(), m_bp_player_UI);
    mp_player_UI = Cast<UPlayer_UI>(p_widget);
    mp_player_UI->AddToViewport(0);
}

void AUI_manager::Set_player_inventory()
{
    p_inventory_manager = GetWorld()->SpawnActor<AInventory_manager>(AInventory_manager::StaticClass(), FTransform::Identity);
    p_inventory_manager->AttachToActor(UGameplayStatics::GetPlayerPawn(GetWorld(),0), FAttachmentTransformRules::KeepRelativeTransform);
}

void AUI_manager::Init_interaction_UI()
{
    // 위젯 컴포넌트 블루프린트 초기화
    auto widget_bp = ConstructorHelpers::FClassFinder<UInteraction_UI>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_Interaction_UI.BP_Interaction_UI_C'"));
    m_interaction_widget_bp = widget_bp.Class;
}

void AUI_manager::Update_interaction_UI(UWidgetComponent* _widget_comp, FString _type)
{
    // 위젯 설정
    _widget_comp->SetWidgetSpace(EWidgetSpace::Screen);
    _widget_comp->SetRelativeLocation(FVector::ZeroVector);
    _widget_comp->SetWidgetClass(m_interaction_widget_bp);

    auto p_interaction_ui = Cast<UInteraction_UI>(_widget_comp->GetWidget());

    if (p_interaction_ui)
        p_interaction_ui->Title_txt->SetText(FText::FromString(_type));
}