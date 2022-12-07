#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogBoxUI.generated.h"

class UTextBlock;
class UListView;

UCLASS()
class UI_PUBG_API UKillLogBoxUI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* PlayerKillTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* CurrentSurvivorTxt;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	UListView* KillLogListView;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};