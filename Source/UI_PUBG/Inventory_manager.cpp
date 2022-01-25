#include "Inventory_manager.h"
#include "Inventory_UI.h"
#include "Item_Slot_UI.h"
#include "UI_manager.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "PUBG_UE4/Global.h"
#include "Characters/Custom_player.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
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

    // 델리게이트에 바인딩
    AGlobal::Get()->dele_set_item_slot_UI.BindUFunction(this, "Update_item_slot_UI");        

    // 캐릭터 관련 초기화
    mp_player = Cast<ACustom_player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    mp_player->dele_open_inventory.BindUFunction(this, "Open_inventory");
    mp_player->dele_close_inventory.BindUFunction(this, "Close_inventory");
    AttachToActor(mp_player, FAttachmentTransformRules::KeepRelativeTransform);
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
    auto BP_inventory_UI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_UI_Inventory.BP_UI_Inventory_C'"));

    if (BP_inventory_UI.Succeeded())
        m_inventory_actor = BP_inventory_UI.Class;
}

void AInventory_manager::Init_inventory_widget()
{
    UUserWidget* p_user_widget = CreateWidget(GetWorld(), m_inventory_actor);
    p_user_widget->AddToViewport(1);
    mp_inventory_ui = Cast<UInventory_UI>(p_user_widget);
}

void AInventory_manager::Open_inventory()
{
    auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    mp_inventory_ui->SetVisibility(ESlateVisibility::Visible);
    player_controller->SetShowMouseCursor(true);
    
    UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetWorld()->GetFirstPlayerController(), mp_inventory_ui, EMouseLockMode::LockAlways);
}

void AInventory_manager::Close_inventory()
{
    auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    mp_inventory_ui->SetVisibility(ESlateVisibility::Hidden);
    player_controller->SetShowMouseCursor(false);
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}

void AInventory_manager::Update_item_slot_UI(Fs_slot_item_data _item_data)
{
    auto test_slot = Cast<UItem_Slot_UI>(CreateWidget(GetWorld(), mp_UI_manager->bp_item_slot_UI));
    test_slot->Set_slot_item_data(_item_data);
    //mp_inventory_ui->Inventory_list_view->AddItem(test_slot);
}