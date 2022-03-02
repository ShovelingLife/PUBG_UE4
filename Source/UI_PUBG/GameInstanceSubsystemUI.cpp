#include "GameInstanceSubsystemUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UObject/ConstructorHelpers.h"

UGameInstanceSubsystemUI::UGameInstanceSubsystemUI()
{
    auto BP_UImanager = ConstructorHelpers::FClassFinder<AUI_manager>(TEXT("Blueprint'/Game/Blueprints/Managers/BP_UI_manager.BP_UI_manager_C'"));

    if (BP_UImanager.Succeeded())
        UImanagerClass = BP_UImanager.Class;
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
    if (UImanagerClass)
        pUImanager = GetWorld()->SpawnActor<AUI_manager>(UImanagerClass);        
}

FVector2D UGameInstanceSubsystemUI::GetDistanceBetweenSlotCursor(UUserWidget* _pWigdet, bool& _bFirst)
{
    if (!_pWigdet)
        return FVector2D::ZeroVector;

    // 마우스 위치를 구함   
    FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

    // 현재 이미지 위치를 구함
    FVector2D imgPos = FVector2D::ZeroVector, dummy_vec;
    auto      cachedGeometry = _pWigdet->GetCachedGeometry();
    USlateBlueprintLibrary::LocalToViewport(GetWorld(), cachedGeometry, FVector2D::ZeroVector, dummy_vec, imgPos);

    // 마우스 좌표 - 이미지 좌표 간 거리
    float posX = mousePos.X - imgPos.X,
          posY = mousePos.Y - imgPos.Y;

    if (FMath::CeilToFloat(imgPos.Y) == 87.f)
        _bFirst = true;

    return FVector2D(posX, posY);
}

bool UGameInstanceSubsystemUI::IsMouseLeftFromUI(FVector2D _Distance, bool _bFirst)
{
    if (_bFirst)
        return ((_Distance.X <= 0.1f) || (_Distance.X >= 225.f) || (_Distance.Y <= -2.f) || (_Distance.Y >= 50.f));

    else
        return ((_Distance.X <= 0.1f) || (_Distance.X >= 225.f) || (_Distance.Y <= 1.5f) || (_Distance.Y >= 55.f));
}
