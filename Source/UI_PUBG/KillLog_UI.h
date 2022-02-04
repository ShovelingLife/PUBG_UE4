#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLog_UI.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class UI_PUBG_API UKillLog_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Killer_name_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* Victim_name_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*	   Killlog_icon_img;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};