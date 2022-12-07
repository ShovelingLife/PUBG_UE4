#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PUBG_UE4/MyEnum.h"
#include "PlayerEffectUI.generated.h"

class UImage;
class UWidgetAnimation;

UCLASS()
class UI_PUBG_API UPlayerEffectUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) 
	UImage* AnimImg;

    UPROPERTY(meta = (BindWidgetAnim), transient) 
	UWidgetAnimation* LongAnim; // ����ź

    UPROPERTY(meta = (BindWidgetAnim), transient) 
	UWidgetAnimation* ShortAnim; // ȭ����
    //UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* InjureAnim; // �λ�
    //UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* FieldAnim; // �ڱ���

	UPROPERTY(VisibleAnywhere, category = UI_Texture) 
	UTexture2D* BurnTex;

	UPROPERTY(VisibleAnywhere, category = UI_Texture) 
	UTexture2D* InjureTex;

	UPROPERTY(VisibleAnywhere, category = UI_Texture) 
	UTexture2D* FieldTex;

public:
	UPlayerEffectUI(const FObjectInitializer& ObjectInitializer);

private:
	void InitTex();

public:
    void PlayAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type);

    void ScheduleDestroyAnim(float WaitTime = 0.f);
};