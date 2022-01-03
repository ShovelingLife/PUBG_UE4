// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "KillLog_Box_UI.generated.h"

/**
 * 
 */
UCLASS()
class UI_PUBG_API UKillLog_Box_UI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Player_kill_text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Current_survivor_text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UListView* KillLog_listview;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};
