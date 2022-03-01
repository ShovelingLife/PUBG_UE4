#include "UI_manager.h"
#include "InteractionUI.h"
#include "InventoryListUI.h"
#include "InventoryManager.h"
#include "PlayerUI.h"
#include "TooltipUI.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

TMap<int, UMaterial*> AUI_manager::MapMainWeaponMat;
TMap<int, UTexture*>  AUI_manager::MapInventoryWeaponTex;
TMap<int, UTexture*>  AUI_manager::MapPlayerTex;

AUI_manager::AUI_manager()
{
    PrimaryActorTick.bCanEverTick = true;
    InitPlayerUI();
    InitInteractionUI();
    InitPlayerUI_Tex();
    InitInventoryWeaponTex();
    InitMainWeaponMat();
}

void AUI_manager::BeginPlay()
{
    Super::BeginPlay();
    
    // 델리게이트 바인딩
    auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());

    if (p_customGameInst)
        p_customGameInst->DeleUpdateInteractionWidgetComp.BindUFunction(this, "UpdateInteractionUI");

    SetPlayerUI();
    InitPlayerInventory();
    Set_weapon_UI();
}

void AUI_manager::InitPlayerUI()
{
    auto bp_playerUI = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_PlayerUI.BP_PlayerUI_C'"));

    if (bp_playerUI.Succeeded())
        mPlayerUI_BP = bp_playerUI.Class;
}

void AUI_manager::InitInteractionUI()
{
    // 위젯 컴포넌트 블루프린트 초기화
    auto widgetBP = ConstructorHelpers::FClassFinder<UInteractionUI>(TEXT("WidgetBlueprint'/Game/Blueprints/UI/BP_InteractionUI.BP_InteractionUI_C'"));
    mInteractionWidgetBP = widgetBP.Class;
}

void AUI_manager::InitPlayerUI_Tex()
{
    for (int i = 0; i < mkArrPlayerUI_TexPath.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString playerUI_path = mkDefaultPlayerUI_path + mkArrPlayerUI_TexPath[i] + "Icon";
        auto    playerUI_tex  = ConstructorHelpers::FObjectFinder<UTexture>(*playerUI_path);

        if (playerUI_tex.Succeeded())
            MapPlayerTex.Add(i, playerUI_tex.Object);
    }
}

void AUI_manager::InitInventoryWeaponTex()
{
    for (int i = 0; i < ADataTableManager::ArrWeaponData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString weaponUI_path = "/Game/UI/WeaponInventoryIcon/" + ADataTableManager::ArrWeaponData[i].IconPath;
        auto    weaponUI_tex  = ConstructorHelpers::FObjectFinder<UTexture>(*weaponUI_path);

        if (weaponUI_tex.Succeeded())
            AUI_manager::MapInventoryWeaponTex.Add(i, weaponUI_tex.Object);
    }
}

void AUI_manager::InitMainWeaponMat()
{
    for (int i = 0; i < ADataTableManager::ArrWeaponData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString weaponUI_path = "/Game/UI/WeaponIcon/" + ADataTableManager::ArrWeaponData[i].SlotIconPath;
        auto    weaponUI_mat  = ConstructorHelpers::FObjectFinder<UMaterial>(*weaponUI_path);

        if (weaponUI_mat.Succeeded())
            AUI_manager::MapMainWeaponMat.Add(i, weaponUI_mat.Object);
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

void AUI_manager::InitPlayerInventory()
{
    pInventoryManager = GetWorld()->SpawnActor<AInventoryManager>(AInventoryManager::StaticClass());
    pInventoryManager->GetRootComponent()->AttachToComponent(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void AUI_manager::SetPlayerUI()
{
    UUserWidget* p_widget = CreateWidget(GetWorld(), mPlayerUI_BP);
    mpPlayer_UI = Cast<UPlayerUI>(p_widget);
    mpPlayer_UI->AddToViewport(0);
}

void AUI_manager::UpdateInteractionUI(UWidgetComponent* _pWidgetComp, FString _Type)
{
    // 위젯 설정
    _pWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    _pWidgetComp->SetRelativeLocation(FVector::ZeroVector);
    _pWidgetComp->SetWidgetClass(mInteractionWidgetBP);

    auto p_interactionUI = Cast<UInteractionUI>(_pWidgetComp->GetWidget());

    if (p_interactionUI)
        p_interactionUI->TitleTxt->SetText(FText::FromString(_Type));
}