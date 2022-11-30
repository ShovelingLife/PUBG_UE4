#include "InteractionComponent.h"
#include "BaseInteraction.h"
#include "CustomGameInstance.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    // 위젯 초기화
    WidgetComp = CreateDefaultSubobject<UWidgetComponent>("InteractionWidgetComp");
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    WidgetComp->SetVisibility(bPlayerNear);
}

void UInteractionComponent::InitInteractionText(FString Str)
{
    // 상호작용 텍스트 == ""일 시
    if (Str.IsEmpty())
        return;

    // 상호작용 텍스트 초기화
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleSetInteractionWidgetComp.ExecuteIfBound(WidgetComp, FString::Printf(TEXT("%s PickUp "), *Str));
}