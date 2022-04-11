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

void UPlayerUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
}