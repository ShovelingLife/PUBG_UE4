/**
 * \file Custom_game_instance.h
 *
 * \brief 게임 인스턴스 = 싱글톤 통용
 *
 * \ingroup PUBG_UE4
 *
 * \author ShovelingLife
 */
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

class UWidgetComponent;

DECLARE_DELEGATE_TwoParams(FDeleUpdateInteractionWidgetComp, UWidgetComponent*, FString)

/**
  * \brief 게임 인스턴스 클래스
 */
UCLASS()
class PUBG_UE4_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    /**
      * \brief 전역 변수
     */
    UPROPERTY() class ASoundManager* pSoundManager;

    FDeleUpdateInteractionWidgetComp  DeleUpdateInteractionWidgetComp;

public:
    UCustomGameInstance();

protected:
    virtual void OnStart() override;
};