#include "Claymore.h"
#include "PUBG_UE4/CustomGameInstance.h"

AClaymore::AClaymore()
{
    Init(EThrowableWeaponType::CLAYMORE);
    mCallBack.BindLambda([&]()
        {
            if (auto p_customGameInst = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
                p_customGameInst->DeleDealPlayerDmg.ExecuteIfBound(WeaponData.Damage);
        });
}