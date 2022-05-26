#include "CoreFarmableItem.h"

// Sets default values
ACoreFarmableItem::ACoreFarmableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoreFarmableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoreFarmableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}