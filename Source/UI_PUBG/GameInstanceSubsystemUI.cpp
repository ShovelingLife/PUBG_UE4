#include "GameInstanceSubsystemUI.h"
#include "TooltipUI.h"
#include "UI_manager.h"
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