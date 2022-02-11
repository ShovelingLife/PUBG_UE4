/**
 * \file Interaction_UI.h
 *
 * \brief 상호작용 UI
 *
 * \ingroup UI_PUBG
 *
 * \author ShovelingLife
 */
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
    /** \brief 상호작용 텍스트 */
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Title_txt;
};