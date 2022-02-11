#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMap_UI.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class UI_PUBG_API UMiniMap_UI : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*	 Magnetic_timer_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*	 Phase_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*		 MiniMap_img;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*		 Warring_img;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* Magnetic_field_bar;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};