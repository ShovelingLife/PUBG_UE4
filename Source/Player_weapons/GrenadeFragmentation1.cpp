#include "GrenadeFragmentation1.h"
#include "PUBG_UE4/CustomGameInstance.h"

AGrenadeFragmentation1::AGrenadeFragmentation1()
{
    Init(EThrowableWeaponType::FRAGMENTATION1);

    mCallBack.BindLambda([&]()
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
                p_customGameInst->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);
        });
}