#include "PUBG_gamemode.h"
#include "Custom_player.h"
#include "Custom_player_controller.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PUBG_UE4/Custom_game_instance.h"
#include "Kismet/GameplayStatics.h"

APUBG_gamemode::APUBG_gamemode()
{
    DefaultPawnClass      = ACustom_player::StaticClass();
    PlayerControllerClass = ACustom_player_controller::StaticClass();
}

void APUBG_gamemode::StartPlay()
{
    Super::StartPlay();
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}