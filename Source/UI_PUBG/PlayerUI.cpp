#include "PlayerUI.h"
#include "MemberStateUI.h"
#include "PlayerStateUI.h"
#include "KillLogBoxUI.h"
#include "MiniMapUI.h"
#include "CurrentWeaponUI.h"
#include "Components/CanvasPanelSlot.h"
#include "Characters/CustomPlayer.h"
#include "Components/Image.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerUI::NativeConstruct()
{
    Super::NativeConstruct();
    mArrImage.Add(CrosshairUpImg);
    mArrImage.Add(CrosshairLeftImg);
    mArrImage.Add(CrosshairRightImg);
    mArrImage.Add(CrosshairDownImg);
}

void UPlayerUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);

    if (auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        auto p_weapon = p_player->GetCurrentWeapon();

        if (p_weapon)
            UpdateCrossHair(); 
        
        for (auto p_image : mArrImage)
             p_image->SetVisibility((p_weapon != nullptr) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}


void UPlayerUI::UpdateCrossHair()
{
    if (auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
    {
        // 플레이어 위치 기반으로 > 공식 통해 78부터 96까지, 범위는 0.05부터 1까지
        auto  playerSpeed = UKismetMathLibrary::Round(p_player->GetCharacterMovement()->Velocity.Size());
        float increaseVal = UKismetMathLibrary::MapRangeClamped(((playerSpeed / 35) * -1.f) + 96.f, 78.f, 96.f, 0.05f, 1.f) - 0.15f;
        FLinearColor color(1.f, 0.f, 0.f, increaseVal);

        // 캐릭터가 이동할 때마다 점 위치를 바꿈
        for (int i = 0; i < mArrImage.Num(); i++)
        {
            mArrImage[i]->SetColorAndOpacity(color);

            if (auto p_canvasPanelSlot = Cast<UCanvasPanelSlot>(mArrImage[i]->Slot))
            {
                float posX = 0.f, posY = 0.f;

                switch (i)
                {
                // 15
                case 0: posY = (increaseVal *  15.f) - 35.f; break; // 위쪽
                case 1: posX = (increaseVal *  15.f) - 35.f; break; // 왼쪽
                case 2: posX = (increaseVal * -15.f) + 35.f; break; // 오른쪽
                case 3: posY = (increaseVal * -15.f) + 35.f; break; // 아래쪽
                }
                p_canvasPanelSlot->SetPosition(FVector2D(posX, posY));
            }
        }
    }
}