#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item_data.generated.h"

// CSV로부터 가지고올 아이템 정보 구조체
USTRUCT()
struct Fs_item_data : public FTableRowBase
{
	GENERATED_BODY()
};

UCLASS()
class PUBG_UE4_API AItem_data : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_data();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};