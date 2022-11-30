#include "InteractionComponent.h"
#include "BaseInteraction.h"
#include "CustomGameInstance.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    // ���� �ʱ�ȭ
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
    // ��ȣ�ۿ� �ؽ�Ʈ == ""�� ��
    if (Str.IsEmpty())
        return;

    // ��ȣ�ۿ� �ؽ�Ʈ �ʱ�ȭ
    if (auto p_customGameInst = UCustomGameInstance::GetInst())
        p_customGameInst->DeleSetInteractionWidgetComp.ExecuteIfBound(WidgetComp, FString::Printf(TEXT("%s PickUp "), *Str));
}