#include "NotificationTextUI.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UNotificationTextUI::NativeConstruct()
{
    Super::NativeConstruct();

    // 애니메이션 델리게이트 바인딩 (첫 설정 1번)
    FWidgetAnimationDynamicEvent widgetAnimationDynamicEvent;
    widgetAnimationDynamicEvent.BindDynamic(this, &UNotificationTextUI::DeleteNotificationText);
    this->BindToAnimationFinished(FadeAnim, widgetAnimationDynamicEvent);
}

void UNotificationTextUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    // ListView에서 아이템 추가할 때마다 설정
    if (auto p_obj = Cast<UNotificationTextUI>(ListItemObject))
    {
        mbErase = false;
        TxtStr = p_obj->TxtStr;
        DeleDeleteNotificationTextUI = p_obj->DeleDeleteNotificationTextUI;
        NotificationTxt->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
        NotificationTxt->SetText(FText::FromString(TxtStr));
        this->PlayAnimation(FadeAnim);
    }
}

void UNotificationTextUI::NativeOnEntryReleased()
{
    // ListView에서 삭제 후 애니메이션이 진행 중일 시 멈춤
    if (this->IsAnimationPlaying(FadeAnim))
    {
        mbErase = true;
        this->StopAnimation(FadeAnim);
    }
}

UFUNCTION() void UNotificationTextUI::DeleteNotificationText()
{
    if (mbErase)
        return;

    DeleDeleteNotificationTextUI.ExecuteIfBound(this);    
}