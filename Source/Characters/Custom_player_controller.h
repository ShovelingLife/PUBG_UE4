/**
 * \file Custom_player_controller.h
 *
 * \brief 캐릭터 컨트롤러 관련
 *
 * \ingroup Characters
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Custom_player_controller.generated.h"

UCLASS()
class CHARACTERS_API ACustom_player_controller : public APlayerController
{
	GENERATED_BODY()

protected:
	/**
	 * \brief 탭키 통해 UI 내비게이션 비활성화
	 */
	virtual void BeginPlay() override;

	/**
	 * \brief 캐릭터-차량 전환 시 컨트롤러 초기화
	 */
	virtual void OnUnPossess() override;
};