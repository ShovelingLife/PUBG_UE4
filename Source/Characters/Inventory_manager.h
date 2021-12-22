// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory_manager.generated.h"

class UUserWidget;
class UInventory_UI;

UCLASS()
class CHARACTERS_API AInventory_manager : public AActor
{
	GENERATED_BODY()

private:
	// UI ฐüทร
	TSubclassOf<UUserWidget> m_user_widget;
	UInventory_UI*			 mp_inventory_ui;

public:
	bool is_opened = false;

public:	
	AInventory_manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Init();

public:
	UInventory_UI* Get_UI() { return mp_inventory_ui; }
};
