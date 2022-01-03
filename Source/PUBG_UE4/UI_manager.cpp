#include "UI_manager.h"
#include "UI_PUBG/Inventory_manager.h"
#include "UI_PUBG/Player_UI.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AUI_manager::AUI_manager()
{
	PrimaryActorTick.bCanEverTick = true;
    Init_player_UI();
}

// Called when the game starts or when spawned
void AUI_manager::BeginPlay()
{
	Super::BeginPlay();
	Set_player_UI();
	Set_player_inventory();
}

// Called every frame
void AUI_manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
    p_inventory_manager->AttachToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), FAttachmentTransformRules::KeepRelativeTransform);
}
