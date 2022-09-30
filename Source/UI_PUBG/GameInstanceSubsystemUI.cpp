#include "GameInstanceSubsystemUI.h"
#include "InventoryManager.h"
#include "ItemSlotUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

UWorld* UGameInstanceSubsystemUI::mpWorld;

UGameInstanceSubsystemUI::UGameInstanceSubsystemUI()
{
    auto BP_UImanager = ConstructorHelpers::FClassFinder<AUI_manager>(TEXT("Blueprint'/Game/1_Blueprints/Managers/BP_UI_manager.BP_UI_manager_C'"));

    if (BP_UImanager.Succeeded())
        mUImanagerClass = BP_UImanager.Class;
}

UGameInstanceSubsystemUI* UGameInstanceSubsystemUI::GetInst()
{
    return UGameplayStatics::GetGameInstance(mpWorld)->GetSubsystem<UGameInstanceSubsystemUI>();
}

void UGameInstanceSubsystemUI::Initialize(FSubsystemCollectionBase& Collection)
{
    TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UGameInstanceSubsystemUI::Tick));
    Super::Initialize(Collection);
    UGameInstanceSubsystemUI::mpWorld = GetWorld();
    InitUImanager();
}

void UGameInstanceSubsystemUI::Deinitialize()
{
    Super::Deinitialize();
}

bool UGameInstanceSubsystemUI::Tick(float DeltaSeconds)
{
    return true;
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

    // 마우스 위치를 구함   
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 현재 이미지 위치를 구함
    FVector2D imgPos = FVector2D::ZeroVector, dummy_vec;
    auto      cachedGeometry = pWigdet->GetCachedGeometry();
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cachedGeometry, FVector2D::ZeroVector, dummy_vec, imgPos);

    // 마우스 좌표 - 이미지 좌표 간 거리
    float posX = mousePos.X - imgPos.X,
          posY = mousePos.Y - imgPos.Y;

    bFirst = (FMath::CeilToFloat(imgPos.Y) == 87.f);
    return FVector2D(posX, posY);
}

bool UGameInstanceSubsystemUI::IsMouseLeftFromUI(FVector2D Distance, bool bFirst)
{
    float minDist = 0.f, maxDist = 0.f;
    minDist = bFirst ? -2.f : -1.5f;
    maxDist = bFirst ? 50.f : 55.f;    
    return ((Distance.X <= 0.1f) || (Distance.X >= 225.f) || (Distance.Y <= minDist) || (Distance.Y >= maxDist));
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