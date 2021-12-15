// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_farmable_item.h"

// Sets default values
ACore_farmable_item::ACore_farmable_item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACore_farmable_item::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACore_farmable_item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

