#include "Molotov.h"
#include "PUBG_UE4/CustomGameInstance.h"

AMolotov::AMolotov() : Super(EThrowableWeaponType::MOLOTOV)
{
    Super::InitSphereComp();
}