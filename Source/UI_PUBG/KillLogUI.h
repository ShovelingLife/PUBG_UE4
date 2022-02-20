#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogUI.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class UI_PUBG_API UKillLogUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* KillerNameTxt;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* VictimNameTxt;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*	   KilllogIconImg;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};