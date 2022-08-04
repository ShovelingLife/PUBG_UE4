#include "CoreFarmableItem.h"
#include "TimerManager.h"

ACoreFarmableItem::ACoreFarmableItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACoreFarmableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ACoreFarmableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}