#include "NotificationTextUI.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UNotificationTextUI::NativeConstruct()
{
    Super::NativeConstruct();

    // �ִϸ��̼� ��������Ʈ ���ε� (ù ���� 1��)
    FWidgetAnimationDynamicEvent widgetAnimationDynamicEvent;
    widgetAnimationDynamicEvent.BindDynamic(this, &UNotificationTextUI::DeleteNotificationText);
    this->BindToAnimationFinished(FadeAnim, widgetAnimationDynamicEvent);
}

void UNotificationTextUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    // ListView���� ������ �߰��� ������ ����
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
    // ListView���� ���� �� �ִϸ��̼��� ���� ���� �� ����
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