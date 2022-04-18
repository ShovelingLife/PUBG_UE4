#include "Molotov.h"
#include "PUBG_UE4/CustomGameInstance.h"

AMolotov::AMolotov()
{
    Init(EThrowableWeaponType::MOLOTOV);
    Super::InitSphereComp();
}