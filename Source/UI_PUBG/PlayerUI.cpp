#include "PlayerUI.h"
#include "MemberStateUI.h"
#include "PlayerStateUI.h"
#include "KillLogBoxUI.h"
#include "MiniMapUI.h"
#include "CurrentWeaponUI.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerUI::UpdateCrosshairVisibility()
{
    using enum EWeaponType;

    TArray<USizeBox*> arrSB_Crosshair
    {
        SB_CrosshairTop,
        SB_CrosshairBottom,
        SB_CrosshairLeft,
        SB_CrosshairRight
    };
    if (pPlayer)
    {
        if (auto pWeaponManager = pPlayer->GetWeaponManager())
        {
            auto currentWeaponType = pWeaponManager->CurrentType;
            auto visibility = (currentWeaponType == FIRST  ||
                               currentWeaponType == SECOND ||
                               currentWeaponType == PISTOL) ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

            for (int i = 0; i < arrSB_Crosshair.Num(); i++)
                 arrSB_Crosshair[i]->SetVisibility(visibility);

            CrosshairCenterImg->SetVisibility(visibility);
            auto cachedPos = CrosshairCenterImg->GetCachedGeometry().GetAbsolutePosition();
            pWeaponManager->ScreenCenterPos = cachedPos / 2;
            //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, (cachedPos / 2).ToString());
        }
    }
}

void UPlayerUI::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    HandleCrosshairScale(TestVel);
}

void UPlayerUI::NativeConstruct()
{
    Super::NativeConstruct();     
    pPlayer = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    GetWorld()->GetTimerManager().SetTimer(TCrosshairHandle, this, &UPlayerUI::HandleCrosshairScale, 1.f / UpdateSec, true);
}

void UPlayerUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    UpdateCrosshairVisibility();
}

void UPlayerUI::HandleCrosshairScale()
{    
    if (pPlayer)
    {
        TestVel = pPlayer->GetVelocity().Size();
        HandleCrosshairScale(TestVel);
    }
}

void UPlayerUI::HandleCrosshairScale(float Vel)
{
    float normalizedVal = UKismetMathLibrary::NormalizeToRange(Vel, 0.1f, MaxVel);
    float fixedVel = FMath::Clamp(normalizedVal, 0.1f, 1.f) * MaxOffset;

    if (auto pTopSlot = Cast<UCanvasPanelSlot>(SB_CrosshairTop->Slot))
        pTopSlot->SetPosition(FVector2D(0.f, -fixedVel));

    if (auto pBottomSlot = Cast<UCanvasPanelSlot>(SB_CrosshairBottom->Slot))
        pBottomSlot->SetPosition(FVector2D(0.f, fixedVel));

    if (auto pLeftSlot = Cast<UCanvasPanelSlot>(SB_CrosshairLeft->Slot))
        pLeftSlot->SetPosition(FVector2D(-fixedVel, 0.f));

    if (auto pRightSlot = Cast<UCanvasPanelSlot>(SB_CrosshairRight->Slot))
        pRightSlot->SetPosition(FVector2D(fixedVel, 0.f));
}