// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Interaction_UI.generated.h"

/**
 * 
 */
UCLASS()
class UI_PUBG_API UInteraction_UI : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* Title_txt;
};
