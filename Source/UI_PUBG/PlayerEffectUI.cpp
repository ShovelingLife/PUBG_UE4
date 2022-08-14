#include "PlayerEffectUI.h"

#include "TimerManager.h"
#include "Animation/WidgetAnimation.h"
#include "Engine/LatentActionManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "PUBG_UE4/SoundManager.h"
#include "Components/Image.h"

UPlayerEffectUI::UPlayerEffectUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    InitTex();
}

void UPlayerEffectUI::InitTex()
{
    const FString path = "Texture2D'/Game/UI/Player/Screen/";
    auto burnTex = ConstructorHelpers::FObjectFinder<UTexture2D>(*(path + "BurnScreen.BurnScreen'"));

    if (burnTex.Succeeded())
        BurnTex = burnTex.Object;

    auto bloodTex = ConstructorHelpers::FObjectFinder<UTexture2D>(*(path + "BloodScreen.BloodScreen'"));

    if (bloodTex.Succeeded())
        InjureTex = bloodTex.Object;

    auto fieldTex = ConstructorHelpers::FObjectFinder<UTexture2D>(*(path + "FieldScreen.FieldScreen'"));

    if (fieldTex.Succeeded())
        FieldTex = fieldTex.Object;
}

void UPlayerEffectUI::PlayAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type)
{
    int index = (int)Type;
    TArray<UTexture2D*> arrTex
    {
        nullptr,
        BurnTex,
        InjureTex,
        FieldTex
    };
    AnimImg->SetBrushFromTexture(arrTex[index]);

    if (Type == EPlayerStateAnimType::INJURED ||
        Type == EPlayerStateAnimType::BURNED)
    {
        if (!this->IsAnimationPlaying(ShortAnim))
            this->PlayAnimation(ShortAnim, StartTime, 0);
    }
    else
    {
        if (!this->IsAnimationPlaying(LongAnim))
        {
            this->PlayAnimation(LongAnim, StartTime);
            ScheduleDestroyAnim(WaitTime - 0.05f);
        }
    }
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
    {
        if (auto p_soundManager = p_customGameInst->pSoundManager)
            p_soundManager->PlayPlayerEffectSound(index);
    }
}

void UPlayerEffectUI::ScheduleDestroyAnim(float WaitTime)
{
    FTimerHandle waitHandle;
    GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
        {
            this->RemoveFromParent();
        }), WaitTime, false);
}