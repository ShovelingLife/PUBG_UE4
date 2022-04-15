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
    TArray<Chaos::Pair<UWidgetAnimation*, UTexture2D*>> arrWidgetAnim
    {
        Chaos::MakePair<UWidgetAnimation*, UTexture2D*>(FlashAnim, nullptr),
        Chaos::MakePair<UWidgetAnimation*, UTexture2D*>(BurnAnim, BurnTex),
        Chaos::MakePair<UWidgetAnimation*, UTexture2D*>(InjureAnim, InjureTex),
        Chaos::MakePair<UWidgetAnimation*, UTexture2D*>(FieldAnim, FieldTex)
    };
    AnimImg->SetBrushFromTexture(arrWidgetAnim[index].Second);

    if (Type == EPlayerStateAnimType::INJURED)
        this->PlayAnimation(arrWidgetAnim[index].First, StartTime, 0);

    else
    {
        this->PlayAnimation(arrWidgetAnim[index].First, StartTime);
        ScheduleDestroyAnim(WaitTime - 0.05f);
    }
    if(auto p_customGameInst=Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (auto p_soundManager = p_customGameInst->pSoundManager)
            p_soundManager->PlayPlayerEffectSound(index);
    }
}