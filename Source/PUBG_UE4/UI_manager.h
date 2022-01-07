#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI_manager.generated.h"

class UPlayer_UI;

UCLASS()
class PUBG_UE4_API AUI_manager : public AActor
{
	GENERATED_BODY()
	
private:
	// ------- ���� ���̰��ִ� ������ -------

	class AGlobal* mp_global =nullptr;

	// �ֻ��� UI ���� ����
	UPlayer_UI*				mp_player_UI;	
	TSubclassOf<UPlayer_UI> m_bp_player_UI;

	// �κ��丮
	TSubclassOf<AActor> m_inventory_manager_actor;

    // ��ȣ�ۿ� UI ����
	UPROPERTY(VisibleAnywhere, Category = Widget_bp)
    TSubclassOf<UUserWidget> m_interaction_widget_bp;

public:
	class AInventory_manager* p_inventory_manager;

public:	
	// Sets default values for this actor's properties
	AUI_manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// ������ �� �ʱ�ȭ �Լ�
private:

	// �ֻ��� �÷��̾� UI �ʱ�ȭ
	void Init_player_UI();

	// �÷��̾� UI�� ���
	void Set_player_UI();

	// �÷��̾� �κ��丮�� ����
	void Set_player_inventory();

	void Update_player_UI(float);

public:
	void Open_or_close_inventory(bool);

	void Init_interaction_UI();

	void Update_interaction_UI(class UWidgetComponent* _widget_comp, FString);
};