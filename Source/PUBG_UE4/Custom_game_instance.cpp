#include "Custom_game_instance.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound_manager.h"

UCustom_game_instance::UCustom_game_instance()
{

}

void UCustom_game_instance::OnStart()
{
    p_sound_manager = GetWorld()->SpawnActor<ASound_manager>(ASound_manager::StaticClass());
}