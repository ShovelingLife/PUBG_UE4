// Fill out your copyright notice in the Description page of Project Settings.

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
	TSubclassOf<UPlayer_UI> m_bp_player_UI;
	UPlayer_UI*				mp_player_UI;	// 최상위 UI 모든걸 포함

	TSubclassOf<AActor> m_inventory_manager_actor;

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
};