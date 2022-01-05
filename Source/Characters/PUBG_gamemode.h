// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PUBG_gamemode.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API APUBG_gamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APUBG_gamemode();

	virtual void StartPlay() override;
};
