#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Custom_player_controller.generated.h"

UCLASS()
class CHARACTERS_API ACustom_player_controller : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void OnUnPossess() override;
};