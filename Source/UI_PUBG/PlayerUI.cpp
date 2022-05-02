#include "PlayerUI.h"
#include "MemberStateUI.h"
#include "PlayerStateUI.h"
#include "KillLogBoxUI.h"
#include "MiniMapUI.h"
#include "CurrentWeaponUI.h"
#include "Characters/CustomPlayer.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UPlayerUI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
}

void UPlayerUI::UpdateAimUI()
{
    if (auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        if (p_player->CurrentState == EPlayerState::AIM)
        {
            FWidgetTransform crosshairImgTrans;
            float mousePosX = 0.f, mousePosY = 0.f;
            CrosshairImg->SetVisibility(ESlateVisibility::Visible);
            UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mousePosX, mousePosY);
            crosshairImgTrans.Translation.Set(mousePosX, mousePosY);
            CrosshairImg->SetRenderTransform(crosshairImgTrans);
        }
        else
            CrosshairImg->SetVisibility(ESlateVisibility::Hidden);
    }
}