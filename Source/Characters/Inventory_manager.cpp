#include "Inventory_manager.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI_PUBG/Inventory_UI.h"

// Sets default values
AInventory_manager::AInventory_manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Init();
}

// Called when the game starts or when spawned
void AInventory_manager::BeginPlay()
{
	Super::BeginPlay();
	
    UUserWidget* p_user_widget = CreateWidget(GetWorld(), m_user_widget);
    p_user_widget->AddToViewport(1);
    mp_inventory_ui = Cast<UInventory_UI>(p_user_widget);
}

// Called every frame
void AInventory_manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (is_opened)
		mp_inventory_ui->SetVisibility(ESlateVisibility::Visible);

	else
		mp_inventory_ui->SetVisibility(ESlateVisibility::Hidden);
}

void AInventory_manager::Init()
{
	ConstructorHelpers::FClassFinder<UUserWidget> INVENTORY_WIDGET(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_UI_Inventory.BP_UI_Inventory_C'"));

	if (INVENTORY_WIDGET.Succeeded())
		m_user_widget = INVENTORY_WIDGET.Class;
}