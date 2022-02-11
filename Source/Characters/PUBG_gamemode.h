/**
 * \file PUBG_gamemode.h
 *
 * \brief 캐릭터 컨트롤러 관련
 *
 * \ingroup Characters
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PUBG_gamemode.generated.h"

UCLASS()
class CHARACTERS_API APUBG_gamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * \brief 기본 플레이어 및 컨트롤러 지정
	 */
	APUBG_gamemode();

	/**
	 * \brief 게임모드로 즉시 전환
	 */
	virtual void StartPlay() override;
};
