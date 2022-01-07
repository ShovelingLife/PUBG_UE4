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
	// ------- 현재 쓰이고있는 변수들 -------

	class AGlobal* mp_global =nullptr;

	// 최상위 UI 모든걸 포함
	UPlayer_UI*				mp_player_UI;	
	TSubclassOf<UPlayer_UI> m_bp_player_UI;

	// 인벤토리
	TSubclassOf<AActor> m_inventory_manager_actor;

    // 상호작용 UI 관련
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

// 생성자 내 초기화 함수
private:

	// 최상위 플레이어 UI 초기화
	void Init_player_UI();

	// 플레이어 UI를 띄움
	void Set_player_UI();

	// 플레이어 인벤토리를 설정
	void Set_player_inventory();

	void Update_player_UI(float);

public:
	void Open_or_close_inventory(bool);

	void Init_interaction_UI();

	void Update_interaction_UI(class UWidgetComponent* _widget_comp, FString);
};