#include "GrenadeStick.h"
#include "PUBG_UE4/CustomGameInstance.h"

AGrenadeStick::AGrenadeStick()
{
    Init(EThrowableWeaponType::STICK);
    mCallBack.BindLambda([&]()
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
                p_customGameInst->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);
        });
}