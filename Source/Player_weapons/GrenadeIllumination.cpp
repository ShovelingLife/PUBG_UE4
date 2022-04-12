#include "GrenadeIllumination.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PUBG_UE4/CustomGameInstance.h"

AGrenadeIllumination::AGrenadeIllumination()
{
    Init(EThrowableWeaponType::ILLUMINATION);
}

void AGrenadeIllumination::Explode()
{
    if (IsPlayerInRadius())
    {
        if (UCustomGameInstance* p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
        {
            float distance = this->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
            float startTime = (distance >= 300) ? 2.5f : 0.f;
            float waitTime = (startTime == 0.f) ? 5.f : 2.5f;
            p_customGameInst->DeleRunEffectAnim.ExecuteIfBound(startTime, waitTime, EPlayerStateAnimType::BLINDED);
        }
    }
}