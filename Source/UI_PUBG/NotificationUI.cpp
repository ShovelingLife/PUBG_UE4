#include "NotificationUI.h"
#include "NotificationTextUI.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

void UNotificationUI::NativeConstruct()
{
    Super::NativeConstruct();
	
	// ó�� ������ ���� ���ε�
	if (auto p_gameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		p_gameInst->DeleSetShootTypeNotificationTxt.BindUFunction(this, "UpdateNotificationText");
}

void UNotificationUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

// ������1) ���� ������ ��� ���Ѿ��� 
void UNotificationUI::UpdateNotificationText(int GunShootType)
{
	const FString str				  = "Current type : ";
	FString		  shootTypeStr		  = "";
	EGunShootType currentGunShootType = (EGunShootType)GunShootType;

	switch (currentGunShootType) // ���� �ݹ� ���¿� ���� 
	{
	case EGunShootType::SINGLE:		 shootTypeStr = "Single";	   break;
	case EGunShootType::BURST:		 shootTypeStr = "Burst";	   break;
	case EGunShootType::CONSECUTIVE: shootTypeStr = "Consecutive"; break;
	}
	// CreateWidget �Լ��� ������ ����� ��� ���� > ������ �ƴ�
	// WidgetTree(UI �ھ�)�� ���μ� ����
	// ���� _0 ���� _3���� �����ϸ� ��Ȱ���� (�����ս� ���� ��� X)
	if (auto p_obj = CreateWidget<UNotificationTextUI>(this, BP_NotificationText))
    {
		// 4���� �� �������� 1�� �߰��� �� ������ ���� (����Ʈ �ִ� ���� 3��) 
        p_obj->TxtStr = str + shootTypeStr;
        p_obj->DeleDeleteNotificationTextUI.BindUFunction(this, "DeleteNotificationTextUI");
        mQueueNotificationText.Enqueue(p_obj);
        NotificationListView->AddItem(p_obj);

        if (NotificationListView->GetNumItems() > 3)
        {
            // ť���� ���� 1�� ���� �� ����Ʈ�� ����� 
            mQueueNotificationText.Pop();
            NotificationListView->ClearListItems();

			// ť�� ����� ������ �ݺ� (���⼭ �ش� ���ҵ��� ������ �� ���ѷ����� ���� ���� ����)
            while (!mQueueNotificationText.IsEmpty())
            {
                UNotificationTextUI* p_notificationTextUI = nullptr;
                mQueueNotificationText.Dequeue(p_notificationTextUI);
                NotificationListView->AddItem(p_notificationTextUI);
            }
			// �Ϸ� �� ť�� ���� ���ҵ��� �����
			for (auto item : NotificationListView->GetListItems())
				 mQueueNotificationText.Enqueue(Cast<UNotificationTextUI>(item));
        }
	}
}	

UFUNCTION() void UNotificationUI::DeleteNotificationTextUI(UNotificationTextUI* pNotificationTextUI)
{
	// �ִϸ��̼��� ������ �� ������ ����
	mQueueNotificationText.Pop();
	NotificationListView->RemoveItem(NotificationListView->GetItemAt(0));
}