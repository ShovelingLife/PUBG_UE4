#include "CustomGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SoundManager.h"

UCustomGameInstance* UCustomGameInstance::inst;

UCustomGameInstance::UCustomGameInstance()
{

}

void UCustomGameInstance::OnStart()
{ 
    if (auto p_world = GetWorld())
    {
        pSoundManager = p_world->SpawnActor<ASoundManager>(ASoundManager::StaticClass());
        UCustomGameInstance::inst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(p_world));
    }
}