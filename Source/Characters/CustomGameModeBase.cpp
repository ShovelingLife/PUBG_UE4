#include "CustomGameModeBase.h"
#include "CustomPlayer.h"
#include "CustomPlayerController.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

ACustomGameModeBase::ACustomGameModeBase()
{
    DefaultPawnClass      = ACustomPlayer::StaticClass();
    PlayerControllerClass = ACustomPlayerController::StaticClass();    
}

void ACustomGameModeBase::StartPlay()
{
    Super::StartPlay();

    if (auto p_World = GetWorld())
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(p_World->GetFirstPlayerController());
}