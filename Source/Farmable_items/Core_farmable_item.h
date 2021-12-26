// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PUBG_UE4/Base_interaction.h"
#include "Core_farmable_item.generated.h"

UCLASS()
class FARMABLE_ITEMS_API ACore_farmable_item : public ABase_interaction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACore_farmable_item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
