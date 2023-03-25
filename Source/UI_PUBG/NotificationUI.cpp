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
		p_gameInst->DeleSetFadingTxt.BindUFunction(this, "UpdateNotificationText");
}

void UNotificationUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UNotificationUI::UpdateNotificationText(FString Msg)
{
	// CreateWidget �Լ��� ������ ����� ��� ���� > ������ �ƴ�
	// WidgetTree(UI �ھ�)�� ���μ� ����
	// ���� _0 ���� _3���� �����ϸ� ��Ȱ���� (�����ս� ���� ��� X)
	if (auto p_obj = CreateWidget<UNotificationTextUI>(this, BP_NotificationText))
    {
		// 4���� �� �������� 1�� �߰��� �� ������ ���� (����Ʈ �ִ� ���� 3��) 
        p_obj->TxtStr = Msg;
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

void UNotificationUI::DeleteNotificationTextUI(UNotificationTextUI* pNotificationTextUI)
{
	// �ִϸ��̼��� ������ �� ������ ����
	mQueueNotificationText.Pop();
	NotificationListView->RemoveItem(NotificationListView->GetItemAt(0));
}