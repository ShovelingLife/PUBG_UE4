#include "Custom_game_instance.h"
#include "UObject/ConstructorHelpers.h"
#include "Global.h"

UCustom_game_instance::UCustom_game_instance()
{

}

void UCustom_game_instance::OnStart()
{
    auto p_global_actor = GetWorld()->SpawnActor<AGlobal>(AGlobal::StaticClass(), FTransform::Identity);
    AGlobal::Set(p_global_actor);
}