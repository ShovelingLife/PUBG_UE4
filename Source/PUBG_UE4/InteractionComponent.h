// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class ABaseInteraction;

class UBoxComponent;
class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_UE4_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    UPROPERTY(VisibleAnywhere, Category = Widget)
    UWidgetComponent* WidgetComp;

    bool    bPlayerNear = false;

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitInteractionText(FString Str);
};
