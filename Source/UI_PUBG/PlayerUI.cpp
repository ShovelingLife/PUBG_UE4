#include "PlayerUI.h"
#include "MemberStateUI.h"
#include "PlayerStateUI.h"
#include "KillLogBoxUI.h"
#include "MiniMapUI.h"
#include "CurrentWeaponUI.h"

void UPlayerUI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerUI::NativeTick(const FGeometry& _InGeometry, float _DeltaTime)
{
    Super::NativeTick(_InGeometry, _DeltaTime);
}