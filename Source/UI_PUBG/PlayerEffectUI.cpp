#include "PlayerEffectUI.h"
#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"
#include "Engine/LatentActionManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/SoundManager.h"

void UPlayerEffectUI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerEffectUI::ScheduleDestroyAnim(float WaitTime)
{
    FTimerHandle waitHandle;
    GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
        {
            this->RemoveFromParent();
        }), WaitTime, false);    
}

void UPlayerEffectUI::PlayAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type)
{
    if (this->IsPlayingAnimation())
        return;

    int index = (int)Type;
    TArray<UWidgetAnimation*> arrWidgetAnim
    {
        FlashAnim,
        BurnAnim,
        InjureAnim,
    };  
    this->PlayAnimation(arrWidgetAnim[index], StartTime);
    ScheduleDestroyAnim(WaitTime);

    if(auto p_customGameInst=Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (auto p_soundManager = p_customGameInst->pSoundManager)
            p_soundManager->PlayPlayerEffectSound(index);
    }
}