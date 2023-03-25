#include "NotificationUI.h"
#include "NotificationTextUI.h"
#include "PUBG_UE4/CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

void UNotificationUI::NativeConstruct()
{
    Super::NativeConstruct();
	
	// 처음 시작할 때만 바인딩
	if (auto p_gameInst = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		p_gameInst->DeleSetFadingTxt.BindUFunction(this, "UpdateNotificationText");
}

void UNotificationUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UNotificationUI::UpdateNotificationText(FString Msg)
{
	// CreateWidget 함수를 여러번 사용할 경우 생성 > 삭제가 아닌
	// WidgetTree(UI 코어)에 노드로서 존재
	// 따라서 _0 부터 _3까지 존재하며 재활용함 (퍼포먼스 저하 우려 X)
	if (auto p_obj = CreateWidget<UNotificationTextUI>(this, BP_NotificationText))
    {
		// 4개일 시 아이템을 1개 추가한 후 강제로 없앰 (리스트 최대 개수 3개) 
        p_obj->TxtStr = Msg;
        p_obj->DeleDeleteNotificationTextUI.BindUFunction(this, "DeleteNotificationTextUI");
        mQueueNotificationText.Enqueue(p_obj);
        NotificationListView->AddItem(p_obj);

        if (NotificationListView->GetNumItems() > 3)
        {
            // 큐에서 원소 1개 삭제 후 리스트에 재삽입 
            mQueueNotificationText.Pop();
            NotificationListView->ClearListItems();

			// 큐가 비워질 때까지 반복 (여기서 해당 원소들을 삽입할 시 무한루프에 빠질 수가 있음)
            while (!mQueueNotificationText.IsEmpty())
            {
                UNotificationTextUI* p_notificationTextUI = nullptr;
                mQueueNotificationText.Dequeue(p_notificationTextUI);
                NotificationListView->AddItem(p_notificationTextUI);
            }
			// 완료 후 큐에 기존 원소들을 재삽입
			for (auto item : NotificationListView->GetListItems())
				 mQueueNotificationText.Enqueue(Cast<UNotificationTextUI>(item));
        }
	}
}	

void UNotificationUI::DeleteNotificationTextUI(UNotificationTextUI* pNotificationTextUI)
{
	// 애니메이션이 끝났을 시 강제로 삭제
	mQueueNotificationText.Pop();
	NotificationListView->RemoveItem(NotificationListView->GetItemAt(0));
}