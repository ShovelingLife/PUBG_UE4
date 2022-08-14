#include "GameInstanceSubsystemUI.h"
#include "InventoryManager.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

UGameInstanceSubsystemUI::UGameInstanceSubsystemUI()
{
    auto BP_UImanager = ConstructorHelpers::FClassFinder<AUI_manager>(TEXT("Blueprint'/Game/Blueprints/Managers/BP_UI_manager.BP_UI_manager_C'"));

    if (BP_UImanager.Succeeded())
        mUImanagerClass = BP_UImanager.Class;
}

void UGameInstanceSubsystemUI::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    InitUImanager();
}

void UGameInstanceSubsystemUI::Deinitialize()
{
    Super::Deinitialize();
}

void UGameInstanceSubsystemUI::InitUImanager()
{
    if (mUImanagerClass)
        pUImanager = GetWorld()->SpawnActor<AUI_manager>(mUImanagerClass);     
}

FVector2D UGameInstanceSubsystemUI::GetDistanceBetweenSlotCursor(UUserWidget* pWigdet, bool& bFirst)
{
    if (!pWigdet)
        return FVector2D::ZeroVector;

    // ���콺 ��ġ�� ����   
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // ���� �̹��� ��ġ�� ����
    FVector2D imgPos = FVector2D::ZeroVector, dummy_vec;
    auto      cachedGeometry = pWigdet->GetCachedGeometry();
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cachedGeometry, FVector2D::ZeroVector, dummy_vec, imgPos);

    // ���콺 ��ǥ - �̹��� ��ǥ �� �Ÿ�
    float posX = mousePos.X - imgPos.X,
          posY = mousePos.Y - imgPos.Y;

    if (FMath::CeilToFloat(imgPos.Y) == 87.f)
        bFirst = true;

    return FVector2D(posX, posY);
}

bool UGameInstanceSubsystemUI::IsMouseLeftFromUI(FVector2D Distance, bool bFirst)
{
    if (bFirst)
        return ((Distance.X <= 0.1f) || (Distance.X >= 225.f) || (Distance.Y <= -2.f) || (Distance.Y >= 50.f));

    else
        return ((Distance.X <= 0.1f) || (Distance.X >= 225.f) || (Distance.Y <= 1.5f) || (Distance.Y >= 55.f));
}

AWeaponManager* UGameInstanceSubsystemUI::GetWeaponManager()
{
    auto p_currentPlayer = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    return (p_currentPlayer) ? p_currentPlayer->GetWeaponManager() : nullptr;
}

AInventoryManager* UGameInstanceSubsystemUI::GetInventoryManager()
{
    return (pUImanager) ? pUImanager->pInventoryManager : nullptr;
}
