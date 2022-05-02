#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "NotificationTextUI.generated.h"

DECLARE_DELEGATE_OneParam(FDeleDeleteNotificationTextUI, UNotificationTextUI*)

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class UI_PUBG_API UNotificationTextUI : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	FDeleDeleteNotificationTextUI DeleDeleteNotificationTextUI;
    UPROPERTY(meta = (BindWidgetAnim), transient) UWidgetAnimation* FadeAnim; // �ؽ�Ʈ ������� ȿ��
	UPROPERTY(meta = (BindWidget)) UTextBlock* NotificationTxt;
	FString TxtStr = "";

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UFUNCTION() void OnAnimationEnded();
};