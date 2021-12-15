// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_bullet.h"

// Sets default values
ACore_bullet::ACore_bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACore_bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACore_bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

