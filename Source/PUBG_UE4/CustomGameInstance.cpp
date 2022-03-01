#include "CustomGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "SoundManager.h"

UCustomGameInstance::UCustomGameInstance()
{

}

void UCustomGameInstance::OnStart()
{ 
    if (auto p_World = GetWorld())
        pSoundManager = p_World->SpawnActor<ASoundManager>(ASoundManager::StaticClass());
}