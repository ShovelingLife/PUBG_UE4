#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player_weapons/WeaponEnum.h"
#include "NotificationUI.generated.h"

class UListView;
class UNotificationTextUI;

UCLASS()
class UI_PUBG_API UNotificationUI : public UUserWidget
{
	GENERATED_BODY()

private:
    TQueue<UNotificationTextUI*> mQueueNotificationText;
    int mCount = 0;

public:
    UPROPERTY(meta = (BindWidget)) UListView* NotificationListView;
    UPROPERTY(EditAnywhere, category = NotificationVariables) TSubclassOf<UNotificationTextUI> BP_NotificationText;

protected:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    UFUNCTION() void UpdateNotificationText(FString Msg);

    UFUNCTION() void DeleteNotificationTextUI(UNotificationTextUI* pNotificationTextUI);
};