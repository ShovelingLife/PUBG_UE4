#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionUI.generated.h"

class UTextBlock;

UCLASS()
class UI_PUBG_API UInteractionUI : public UUserWidget
{
	GENERATED_BODY()

public:
// ------- 상호작용 텍스트 -------
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* TitleTxt;
};