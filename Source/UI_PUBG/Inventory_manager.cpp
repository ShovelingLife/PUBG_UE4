#include "Inventory_manager.h"
#include "Inventory_UI.h"
#include "Characters/Custom_player.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AInventory_manager::AInventory_manager()
{
    PrimaryActorTick.bCanEverTick = true;

    // 인벤토리 위젯 초기화
    auto BP_inventory_UI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_UI_Inventory.BP_UI_Inventory_C'"));

    if (BP_inventory_UI.Succeeded())
        m_inventory_actor = BP_inventory_UI.Class;
}

void AInventory_manager::BeginPlay()
{
    Super::BeginPlay();
    mp_custom_player = Cast<ACustom_player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    Init_inventory_widget();
}

void AInventory_manager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Check_inventory_state();
}

void AInventory_manager::Init_inventory_widget()
{
    UUserWidget* p_user_widget = CreateWidget(GetWorld(), m_inventory_actor);
    p_user_widget->AddToViewport(1);
    mp_inventory_ui = Cast<UInventory_UI>(p_user_widget);
}

void AInventory_manager::Check_inventory_state()
{
    auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ESlateVisibility visibility = ESlateVisibility::Hidden;

    if (mp_custom_player->is_inventory_opened)
        visibility = ESlateVisibility::Visible;

    mp_inventory_ui->SetVisibility(visibility);
    player_controller->SetShowMouseCursor(is_opened);
}