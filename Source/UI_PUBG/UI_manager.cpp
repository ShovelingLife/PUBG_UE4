#include "UI_manager.h"
#include "Interaction_UI.h"
#include "Inventory_list_UI.h"
#include "Inventory_manager.h"
#include "Player_UI.h"
#include "PUBG_UE4/Global.h"
#include "PUBG_UE4/Data_table_manager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

TMap<int, UMaterial*> AUI_manager::map_main_weapon_ui_mat;
TMap<int, UTexture*>  AUI_manager::map_inventory_weapon_ui_tex;
TMap<int, UTexture*>  AUI_manager::map_player_ui_tex;

AUI_manager::AUI_manager()
{
    PrimaryActorTick.bCanEverTick = true;
    Init_player_UI();
    Init_interaction_UI();
    Init_player_UI_tex();
    Init_inventory_weapon_UI_tex();
    Init_main_weapon_UI_mat();
}

// Called when the game starts or when spawned
void AUI_manager::BeginPlay()
{
    Super::BeginPlay();
    
    // 델리게이트 바인딩
    mp_global = AGlobal::Get();
    mp_global->dele_update_interaction_widget_comp.BindUFunction(this, "Update_interaction_UI");

    Set_player_UI();
    Init_player_inventory();
    Set_weapon_UI();
}

// Called every frame
void AUI_manager::Tick(float _delta_time)
{
    Super::Tick(_delta_time);
}

void AUI_manager::Init_player_UI()
{
    auto BP_player_UI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_Player_UI.BP_Player_UI_C'"));

    if (BP_player_UI.Succeeded())
        m_bp_player_UI = BP_player_UI.Class;
}

void AUI_manager::Init_interaction_UI()
{
    // 위젯 컴포넌트 블루프린트 초기화
    auto widget_bp = ConstructorHelpers::FClassFinder<UInteraction_UI>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_Interaction_UI.BP_Interaction_UI_C'"));
    m_interaction_widget_bp = widget_bp.Class;
}

void AUI_manager::Init_player_UI_tex()
{
    for (int i = 0; i < mk_arr_player_UI_tex_path.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString player_ui_path = mk_default_player_UI_path + mk_arr_player_UI_tex_path[i] + "_icon";
        auto    player_ui_tex  = ConstructorHelpers::FObjectFinder<UTexture>(*player_ui_path);

        if (player_ui_tex.Succeeded())
            map_player_ui_tex.Add(i, player_ui_tex.Object);
    }
}

void AUI_manager::Set_player_UI()
{
    UUserWidget* p_widget = CreateWidget(GetWorld(), m_bp_player_UI);
    mp_player_UI = Cast<UPlayer_UI>(p_widget);
    mp_player_UI->AddToViewport(0);
}

void AUI_manager::Init_player_inventory()
{
    p_inventory_manager = GetWorld()->SpawnActor<AInventory_manager>(AInventory_manager::StaticClass());
    p_inventory_manager->GetRootComponent()->AttachToComponent(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void AUI_manager::Init_inventory_weapon_UI_tex()
{
    for (int i = 0; i < AData_table_manager::arr_weapon_data.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString weapon_ui_path = "/Game/UI/Weapon_inventory_icon/" + AData_table_manager::arr_weapon_data[i].weapon_icon_path;
        auto    weapon_ui_tex  = ConstructorHelpers::FObjectFinder<UTexture>(*weapon_ui_path);

        if (weapon_ui_tex.Succeeded())
            AUI_manager::map_inventory_weapon_ui_tex.Add(i, weapon_ui_tex.Object);
    }
}

void AUI_manager::Init_main_weapon_UI_mat()
{
    for (int i = 0; i < AData_table_manager::arr_weapon_data.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString weapon_ui_path = "/Game/UI/Weapon_icon/" + AData_table_manager::arr_weapon_data[i].weapon_slot_icon_path;
        auto    weapon_ui_mat  = ConstructorHelpers::FObjectFinder<UMaterial>(*weapon_ui_path);

        if (weapon_ui_mat.Succeeded())
            AUI_manager::map_main_weapon_ui_mat.Add(i, weapon_ui_mat.Object);
    }
    //for (int i = 0; i < MAX_OTHER_WEAPON_COUNT; i++)
    //{
    //    // 리소스를 불러온 후 데이터 테이블에 대입
    //    FString weapon_ui_path = "/Game/UI/Weapon_icon/" + AData_table_manager::arr_other_weapon_data[i].weapon_slot_icon_path;
    //    auto    weapon_ui_mat = ConstructorHelpers::FObjectFinder<UMaterial>(*weapon_ui_path);

    //    if (weapon_ui_mat.Succeeded())
    //        map_main_weapon_ui_mat.Add(i, weapon_ui_mat.Object);
    //}
}

void AUI_manager::Set_weapon_UI()
{
    /*for (int i = 0; i < MAX_WEAPON_COUNT; i++)
    {
    }*/
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