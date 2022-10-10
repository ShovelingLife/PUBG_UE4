#include "WeaponSocketUI.h"
#include "Characters/CustomPlayer.h"
#include "Player_weapons/WeaponManager.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UWeaponSocketUI::NativeConstruct()
{
    Super::NativeConstruct();

    FString name = "";
    this->GetName(name);

    if      (name.Contains("Second"))
             WeaponIdx = 2;

    else if (name.Contains("Pistol"))
             WeaponIdx = 3;
}

void UWeaponSocketUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    Super::NativeTick(InGeometry, DeltaTime);
    auto p_player = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    
    if (p_player)
    {
        if (auto p_weaponManager = p_player->GetWeaponManager())
        {
            // 메인 슬롯 UI 변경
            auto idx = (int)AttachmentType; 
            // GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::FromInt(idx));

            if (idx < p_weaponManager->arrAttachment.Num())
            {
                auto p_attachment = p_weaponManager->arrAttachment[idx];
                ItemImg->SetVisibility((p_attachment) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
            }
            // 배경 UI 변경
            ACoreWeapon* p_weapon = nullptr;

            switch (WeaponIdx)
            {
            case 1: p_weapon = p_weaponManager->pFirstGun;  break;
            case 2: p_weapon = p_weaponManager->pSecondGun; break;
            case 3: p_weapon = p_weaponManager->pPistol;    break;
            }
            ItemBorder->SetVisibility((p_weapon) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}