/**
 * \file Custom_player_controller.h
 *
 * \brief ĳ���� ��Ʈ�ѷ� ����
 *
 * \ingroup Characters
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class CHARACTERS_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/**
	 * \brief ��Ű ���� UI ������̼� ��Ȱ��ȭ
	 */
	virtual void BeginPlay() override;

	/**
	 * \brief ĳ����-���� ��ȯ �� ��Ʈ�ѷ� �ʱ�ȭ
	 */
	virtual void OnUnPossess() override;
};