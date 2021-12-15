// Fill out your copyright notice in the Description page of Project Settings.


#include "TCP_IOCP_SERVER.h"
#include "UI_PUBG/Player_UI.h"

// Sets default values
ATCP_IOCP_SERVER::ATCP_IOCP_SERVER()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATCP_IOCP_SERVER::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATCP_IOCP_SERVER::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

