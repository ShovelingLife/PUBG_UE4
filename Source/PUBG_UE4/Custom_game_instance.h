#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Custom_game_instance.generated.h"

UCLASS()
class PUBG_UE4_API UCustom_game_instance : public UGameInstance
{
	GENERATED_BODY()

public:
    UCustom_game_instance();

protected:
    virtual void OnStart() override;
};