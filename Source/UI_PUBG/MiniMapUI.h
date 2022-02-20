#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapUI.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class UI_PUBG_API UMiniMapUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*	 MagneticTimerTxt;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*	 PhaseTxt;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*		 MiniMapImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*		 WarringImg;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* MagneticFieldBar;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};