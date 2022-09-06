﻿#include "UI_manager.h"
#include "InteractionUI.h"
#include "InventoryListUI.h"
#include "InventoryManager.h"
#include "PlayerUI.h"
#include "PlayerEffectUI.h"
#include "TooltipUI.h"
#include "Characters/CustomPlayer.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/DataTableManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

TMap<int, UTexture*>  AUI_manager::MapPlayerIcon;
TMap<int, UMaterial*> AUI_manager::MapMainWeaponIcon;
TMap<int, UTexture*>  AUI_manager::MapWeaponIcon;
TMap<int, UTexture*>  AUI_manager::MapOtherWeaponIcon;
TMap<int, UTexture*>  AUI_manager::MapWeaponAttachmentIcon;
TMap<int, UTexture*>  AUI_manager::MapAmmoBoxIcon;
TMap<int, UTexture*>  AUI_manager::MapUtilityIcon;

AUI_manager::AUI_manager()
{
    PrimaryActorTick.bCanEverTick = true;
    InitPlayerUI();
    InitPlayerEffectUI();
    InitPlayerEffectUI();
    InitInteractionUI();
    InitPlayerIcon();
    InitInventoryWeaponIcon();
    InitMainWeaponIcon();
    InitUtilityIcon();
}

UTexture2D* AUI_manager::GetTexture2D(FsSlotItemData ItemData)
{
    UTexture* weaponTex = nullptr;
    FString   category = ItemData.Category;
    int       imageIndex = ItemData.ImageIndex;

    if (imageIndex >= MapOtherWeaponIcon.Num())
        return nullptr;

    // 투척류 또는 근접무기일 시
    if      (category == "Throwable" ||
             category == "Melee")
             weaponTex = MapOtherWeaponIcon[imageIndex];

    // 총기 무기일 시
    else if (category == "Gun")
             weaponTex = MapWeaponIcon[imageIndex];

    // 무기 부속품일 시
    else if (category == "Attachment")
             weaponTex = MapWeaponAttachmentIcon[imageIndex];

    else if (category == "AmmoBox")
             weaponTex = MapAmmoBoxIcon[imageIndex];

    return Cast<UTexture2D>(weaponTex);
}

UTexture2D* AUI_manager::GetTexture2D(int Index, FString Type /* = "" */)
{
    // 총기류
    if (Type == "Gun")
        return (Index < MapWeaponIcon.Num()) ? Cast<UTexture2D>(MapWeaponIcon[Index]) : nullptr;

    // 투척류
    else if (Type == "Explosive")
        return (Index < MapOtherWeaponIcon.Num()) ? Cast<UTexture2D>(MapOtherWeaponIcon[Index]) : nullptr;

    // 기타 물품
    else if (Type == "Utility")
        return (Index < MapUtilityIcon.Num()) ? Cast<UTexture2D>(MapUtilityIcon[Index]) : nullptr;

    else
        return nullptr;
}

UMaterial* AUI_manager::GetMaterial(int Index)
{
    return (Index < MapMainWeaponIcon.Num()) ? MapMainWeaponIcon[Index] : nullptr;
}

void AUI_manager::BeginPlay()
{
    Super::BeginPlay();
    
    BindDelegate();
    SetPlayerUI();
    InitPlayerInventory();
    SetWeaponUI();
}

void AUI_manager::BindDelegate()
{
    // 델리게이트 바인딩
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
    {
        p_customGameInst->DeleUpdateInteractionWidgetComp.BindUFunction(this, "UpdateInteractionUI");
        p_customGameInst->DeleRunEffectAnim.BindUFunction(this, "RunEffectAnim");
        p_customGameInst->DeleKillUI_Anim.BindUFunction(this, "KillAnim");
    }
}

void AUI_manager::InitInteractionUI()
{
    // 위젯 컴포넌트 블루프린트 초기화
    ConstructorHelpers::FClassFinder<UInteractionUI> widgetBP(TEXT("WidgetBlueprint'/Game/1_Blueprints/UI/BP_InteractionUI.BP_InteractionUI_C'"));

    if (widgetBP.Succeeded())
        mInteractionWidgetBP = widgetBP.Class;
}

void AUI_manager::InitPlayerUI()
{
    ConstructorHelpers::FClassFinder<UUserWidget> bp_playerUI(TEXT("WidgetBlueprint'/Game/1_Blueprints/UI/BP_PlayerUI.BP_PlayerUI_C'"));

    if (bp_playerUI.Succeeded())
        mPlayerUI_BP = bp_playerUI.Class;
}

void AUI_manager::InitPlayerEffectUI()
{
    ConstructorHelpers::FClassFinder<UUserWidget> bp_playerEffectUI(TEXT("WidgetBlueprint'/Game/1_Blueprints/UI/BP_PlayerEffectUI.BP_PlayerEffectUI_C'"));

    if (bp_playerEffectUI.Succeeded())
        mPlayerEffectUI_BP = bp_playerEffectUI.Class;
}

void AUI_manager::InitPlayerIcon()
{
    for (int i = 0; i < mkArrShootType.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString playerUI_path = "/Game/UI/Player/State/" + mkArrShootType[i] + "Icon";
        ConstructorHelpers::FObjectFinder<UTexture> playerUI_tex(*playerUI_path);

        if (playerUI_tex.Succeeded())
            MapPlayerIcon.Add(i, playerUI_tex.Object);
    }
}

void AUI_manager::InitMainWeaponIcon()
{
    auto arrWeaponData = ADataTableManager::ArrWeaponData;

    for (int i = 0; i < arrWeaponData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString weaponUI_path = "/Game/UI/WeaponIcon/T_" + arrWeaponData[i].SlotIconPath;
        ConstructorHelpers::FObjectFinder<UMaterial> weaponUI_mat(*weaponUI_path);

        if (weaponUI_mat.Succeeded())
            MapMainWeaponIcon.Add(i, weaponUI_mat.Object);
    }
}

void AUI_manager::InitInventoryWeaponIcon()
{
    const FString inventoryWeaponUI_Path = "/Game/UI/InventoryWeaponIcon/";

    // 총기 아이콘 초기화
    auto arrWeaponData = ADataTableManager::ArrWeaponData;

    for (int i = 0; i < arrWeaponData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString UI_path = inventoryWeaponUI_Path + "T_" + arrWeaponData[i].IconPath;
        ConstructorHelpers::FObjectFinder<UTexture> UI_tex(*UI_path);

        if (UI_tex.Succeeded())
            MapWeaponIcon.Add(i, UI_tex.Object);
    }
    // 근접 및 투척류 아이콘 초기화
    auto arrOtherWeaponData = ADataTableManager::ArrOtherWeaponData;

    for (int i = 0; i < arrOtherWeaponData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString UI_path = inventoryWeaponUI_Path + arrOtherWeaponData[i].Type + "_icon";
        ConstructorHelpers::FObjectFinder<UTexture> UI_tex(*UI_path);

        if (UI_tex.Succeeded())
            MapOtherWeaponIcon.Add(i, UI_tex.Object);
    }
    // 부속품 아이콘 초기화
    auto arrWeaponAttachmentData = ADataTableManager::ArrWeaponAttachmentData;

    for (int i = 0; i < arrWeaponAttachmentData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString UI_path = inventoryWeaponUI_Path + "T_" + arrWeaponAttachmentData[i].Type + "_Icon";
        ConstructorHelpers::FObjectFinder<UTexture> UI_tex(*UI_path);

        if (UI_tex.Succeeded())
            MapWeaponAttachmentIcon.Add(i, UI_tex.Object);
    }
}

void AUI_manager::InitUtilityIcon()
{
    // 총알 박스 UI 초기화
    auto arrAmmoBoxData = ADataTableManager::ArrWeaponBulletData;

    for (int i = 0; i < arrAmmoBoxData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString UI_path = "/Game/UI/AmmoBoxIcon/AmmoBox" + arrAmmoBoxData[i].Type + "_Icon";
        ConstructorHelpers::FObjectFinder<UTexture> UI_tex(*UI_path);

        if (UI_tex.Succeeded())
            MapAmmoBoxIcon.Add(i, UI_tex.Object);
    }
    // 그 외 아이콘 초기화
    auto arrUtilityData = ADataTableManager::ArrUtilityData;

    for (int i = 0; i < arrUtilityData.Num(); i++)
    {
        // 리소스를 불러온 후 데이터 테이블에 대입
        FString UI_path = "/Game/UI/UtilityIcon/T_" + arrUtilityData[i].Type;
        ConstructorHelpers::FObjectFinder<UTexture> UI_tex(*UI_path);

        if (UI_tex.Succeeded())
            MapUtilityIcon.Add(i, UI_tex.Object);
    }
}

void AUI_manager::SetWeaponUI()
{
    /*for (int i = 0; i < MAX_WEAPON_COUNT; i++)
    {
    }*/
}

void AUI_manager::InitPlayerInventory()
{
    pInventoryManager = GetWorld()->SpawnActor<AInventoryManager>(AInventoryManager::StaticClass());
    auto p_player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (pInventoryManager &&
        p_player)
        pInventoryManager->GetRootComponent()->AttachToComponent(p_player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void AUI_manager::SetPlayerUI()
{
    // 플레이어 UI
    UUserWidget* p_playerUI_Widget = CreateWidget(GetWorld(), mPlayerUI_BP);

    if (p_playerUI_Widget)
    {
        mpPlayer_UI = Cast<UPlayerUI>(p_playerUI_Widget);
        mpPlayer_UI->AddToViewport(0);
    }
    // 플레이어 상태 UI
    UUserWidget* p_playerEffectUI_Widget = CreateWidget(GetWorld(), mPlayerEffectUI_BP);

    if (p_playerEffectUI_Widget)
        pPlayerEffect_UI = Cast<UPlayerEffectUI>(p_playerEffectUI_Widget);
}

void AUI_manager::UpdateInteractionUI(UWidgetComponent* WidgetComp, FString Type)
{
    if (!WidgetComp)
        return;

    // 위젯 설정
    WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComp->SetRelativeLocation(FVector::ZeroVector);
    WidgetComp->SetWidgetClass(mInteractionWidgetBP);

    auto p_interactionUI = Cast<UInteractionUI>(WidgetComp->GetWidget());

    if (p_interactionUI)
        p_interactionUI->TitleTxt->SetText(FText::FromString(Type));
}

void AUI_manager::RunEffectAnim(float  StartTime, float WaitTime, EPlayerStateAnimType Type)
{
    if (pPlayerEffect_UI &&
        !pPlayerEffect_UI->IsAnyAnimationPlaying())
    {
        pPlayerEffect_UI->AddToViewport(3);
        pPlayerEffect_UI->PlayAnim(StartTime, WaitTime, Type);
    }
}

void AUI_manager::KillAnim()
{
    if (pPlayerEffect_UI)
    {
        pPlayerEffect_UI->StopAllAnimations();
        pPlayerEffect_UI->RemoveFromParent();
    }
}