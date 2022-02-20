#include "CustomGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "SoundManager.h"

UCustomGameInstance::UCustomGameInstance()
{

}

void UCustomGameInstance::OnStart()
{
    pSoundManager = GetWorld()->SpawnActor<ASoundManager>(ASoundManager::StaticClass());
}