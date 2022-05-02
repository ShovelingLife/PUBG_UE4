#include "NotificationTextUI.h"
#include "Components/TextBlock.h"

void UNotificationTextUI::NativeConstruct()
{
    Super::NativeConstruct();    
    //this->PlayAnimation(FadeAnim);
    FWidgetAnimationDynamicEvent widgetAnimationDynamicEvent;
    widgetAnimationDynamicEvent.BindDynamic(this, &UNotificationTextUI::OnAnimationEnded);
}

void UNotificationTextUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (auto p_obj = Cast<UNotificationTextUI>(ListItemObject))
    {
        TxtStr = p_obj->TxtStr;
        NotificationTxt->SetText(FText::FromString(TxtStr));
    }
}

UFUNCTION() void UNotificationTextUI::OnAnimationEnded()
{
    SetRenderOpacity(1.f);
    DeleDeleteNotificationTextUI.ExecuteIfBound(this);
}