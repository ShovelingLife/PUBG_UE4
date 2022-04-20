#include "InventoryManager.h"
#include "InventoryListUI.h"
#include "InventoryUI.h"
#include "ItemSlotUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Texture2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AInventoryManager::AInventoryManager()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
    RootComponent = SceneComp;

    InitInventoryUI();
}

void AInventoryManager::BeginPlay()
{
    Super::BeginPlay();
    InitInventoryWidget();

    // 캐릭터 관련 초기화
    if(auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
    {
        p_player->DeleOpenInventory.BindUFunction(this, "OpenInventory");
        p_player->DeleCloseInventory.BindUFunction(this, "CloseInventory");
        AttachToActor(p_player, FAttachmentTransformRules::KeepRelativeTransform);
    }
}

void AInventoryManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AInventoryManager::InitInventoryUI()
{
    // 인벤토리 위젯 초기화
    auto BP_inventoryUI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_InventoryUI.BP_InventoryUI_C'"));

    if (BP_inventoryUI.Succeeded())
        this->BP_InventoryUI = BP_inventoryUI.Class;
}

void AInventoryManager::InitInventoryWidget()
{
    UUserWidget* p_userWidget = CreateWidget(GetWorld(), BP_InventoryUI);
    p_userWidget->AddToViewport(1);
    pInventoryUI = Cast<UInventoryUI>(p_userWidget);
}

void AInventoryManager::OpenInventory()
{
    auto p_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    pInventoryUI->SetVisibility(ESlateVisibility::Visible);
    p_playerController->SetShowMouseCursor(true);    
    UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetWorld()->GetFirstPlayerController(), pInventoryUI, EMouseLockMode::LockAlways);
}

void AInventoryManager::CloseInventory()
{
    auto p_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    pInventoryUI->SetVisibility(ESlateVisibility::Hidden);
    p_playerController->SetShowMouseCursor(false);
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}

UInventoryListUI* AInventoryManager::GetInventoryListUI()
{
    return pInventoryUI->InventoryListUI;
}