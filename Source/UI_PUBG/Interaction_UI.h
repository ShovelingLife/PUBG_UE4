#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interaction_UI.generated.h"

class UTextBlock;

UCLASS()
class UI_PUBG_API UInteraction_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Title_txt;
};
