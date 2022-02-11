#include "Player_UI.h"
#include "Member_State_UI.h"
#include "Player_State_UI.h"
#include "KillLog_Box_UI.h"
#include "MiniMap_UI.h"
#include "Current_Weapon_UI.h"

void UPlayer_UI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayer_UI::NativeTick(const FGeometry& _my_geometry, float _delta_time)
{
    Super::NativeTick(_my_geometry, _delta_time);
}