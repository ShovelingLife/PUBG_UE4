#include "CustomGameModeBase.h"
#include "CustomPlayer.h"
#include "CustomPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"

ACustomGameModeBase::ACustomGameModeBase()
{
    DefaultPawnClass      = ACustomPlayer::StaticClass();
    PlayerControllerClass = ACustomPlayerController::StaticClass();    
}

void ACustomGameModeBase::StartPlay()
{
    Super::StartPlay();
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}