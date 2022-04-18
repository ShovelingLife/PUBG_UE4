#include "GrenadeFragmentation2.h"
#include "PUBG_UE4/CustomGameInstance.h"

AGrenadeFragmentation2::AGrenadeFragmentation2()
{
    Init(EThrowableWeaponType::FRAGMENTATION2);

    mCallBack.BindLambda([&]()
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
                p_customGameInst->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);
        });
}