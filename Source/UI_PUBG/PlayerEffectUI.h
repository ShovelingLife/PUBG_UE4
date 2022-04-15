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
	UPROPERTY(meta = (BindWidget)) class UImage* AnimImg;
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* FlashAnim; // º∂±§≈∫
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* BurnAnim; // »≠ø∞∫¥
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* InjureAnim; // ∫ŒªÛ
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* FieldAnim; // ¿⁄±‚¿Â

	UPROPERTY(VisibleAnywhere, category = UI_Texture) UTexture2D* BurnTex;
	UPROPERTY(VisibleAnywhere, category = UI_Texture) UTexture2D* InjureTex;
	UPROPERTY(VisibleAnywhere, category = UI_Texture) UTexture2D* FieldTex;

public:
	UPlayerEffectUI(const FObjectInitializer& ObjectInitializer);

private:
	void InitTex();

	void ScheduleDestroyAnim(float WaitTime);

public:
	void PlayAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type);
};