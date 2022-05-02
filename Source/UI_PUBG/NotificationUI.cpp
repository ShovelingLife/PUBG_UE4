#include "NotificationUI.h"
#include "NotificationTextUI.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

void UNotificationUI::NativeConstruct()
{
    Super::NativeConstruct();
	
	if (auto p_gameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		p_gameInst->DeleSetShootTypeNotificationTxt.BindUFunction(this, "UpdateNotificationText");
}

void UNotificationUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

	/*for (auto item : NotificationListView->GetListItems())
	{
		if (auto p_notificationText = Cast<UNotificationTextUI>(item))
		{
			if (!p_notificationText->IsAnyAnimationPlaying())
				NotificationListView->RemoveItem(p_notificationText);
		}
	}*/
}

void UNotificationUI::UpdateNotificationText(int GunShootType)
{
	const FString str	 = "Current type : ";
	FString shootTypeStr = "";
	EGunShootType currentGunShootType = (EGunShootType)GunShootType;

	switch (currentGunShootType)
	{
	case EGunShootType::SINGLE:		 shootTypeStr = str + "Single";		 break;
	case EGunShootType::BURST:		 shootTypeStr = str + "Burst";		 break;
	case EGunShootType::CONSECUTIVE: shootTypeStr = str + "Consecutive"; break;
	}
	if (auto p_obj = CreateWidget<UNotificationTextUI>(this, BP_NotificationText))
    {
		p_obj->TxtStr = shootTypeStr;
        p_obj->DeleDeleteNotificationTextUI.BindUFunction(this, "DeleteNotificationTextUI");
        mQueueNotificationText.Enqueue(p_obj);
        NotificationListView->AddItem(p_obj);

        if (NotificationListView->GetNumItems() > 3)
        {
            mQueueNotificationText.Pop();
            NotificationListView->ClearListItems();

            while (!mQueueNotificationText.IsEmpty())
            {
                UNotificationTextUI* p_notificationTextUI = nullptr;
                mQueueNotificationText.Dequeue(p_notificationTextUI);
                NotificationListView->AddItem(p_notificationTextUI);
            }
			for (auto item : NotificationListView->GetListItems())
				mQueueNotificationText.Enqueue(Cast<UNotificationTextUI>(item));
        }
	}
}

UFUNCTION() void UNotificationUI::DeleteNotificationTextUI(UNotificationTextUI* pNotificationTextUI)
{
	NotificationListView->RemoveItem(pNotificationTextUI);
}