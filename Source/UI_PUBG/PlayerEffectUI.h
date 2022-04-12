#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PUBG_UE4/MyEnum.h"
#include "PlayerEffectUI.generated.h"

class UWidgetAnimation;

UCLASS()
class UI_PUBG_API UPlayerEffectUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* FlashAnim; // ¼¶±¤Åº
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* BurnAnim; // È­¿°º´
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* InjureAnim; // ºÎ»ó

protected:
	virtual void NativeConstruct() override;

private:
	void ScheduleDestroyAnim(float WaitTime);

public:
	void PlayAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type);
};