#include "Inventory_manager.h"
#include "Inventory_UI.h"
#include "Item_Slot_UI.h"
#include "UI_manager.h"
#include "Characters/Custom_player.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Texture2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AInventory_manager::AInventory_manager()
{
    PrimaryActorTick.bCanEverTick = true;
    mp_scene_comp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_comp"));
    RootComponent = mp_scene_comp;

    Init_inventory_UI();
}

void AInventory_manager::BeginPlay()
{
    Super::BeginPlay();
    Init_inventory_widget();

    // 캐릭터 관련 초기화
    auto p_player = Cast<ACustom_player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    p_player->dele_open_inventory.BindUFunction(this, "Open_inventory");
    p_player->dele_close_inventory.BindUFunction(this, "Close_inventory");
    AttachToActor(p_player, FAttachmentTransformRules::KeepRelativeTransform);
}

void AInventory_manager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!mp_UI_manager)
        mp_UI_manager = Cast<AUI_manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUI_manager::StaticClass()));
}

void AInventory_manager::Init_inventory_UI()
{
    // 인벤토리 위젯 초기화
    auto BP_inventory_UI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_Inventory_UI.BP_Inventory_UI_C'"));

    if (BP_inventory_UI.Succeeded())
        m_inventory_actor = BP_inventory_UI.Class;
}

void AInventory_manager::Init_inventory_widget()
{
    UUserWidget* p_user_widget = CreateWidget(GetWorld(), m_inventory_actor);
    p_user_widget->AddToViewport(1);
    p_inventory_UI = Cast<UInventory_UI>(p_user_widget);
}

void AInventory_manager::Open_inventory()
{
    auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    p_inventory_UI->SetVisibility(ESlateVisibility::Visible);
    player_controller->SetShowMouseCursor(true);
    
    UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetWorld()->GetFirstPlayerController(), p_inventory_UI, EMouseLockMode::LockAlways);
}

void AInventory_manager::Close_inventory()
{
    auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    p_inventory_UI->SetVisibility(ESlateVisibility::Hidden);
    player_controller->SetShowMouseCursor(false);
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}