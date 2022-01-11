#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global.generated.h"

class UWidgetComponent;
class AData_table_manager;
class ASound_manager;
class UCustom_game_instance;

DECLARE_DELEGATE_TwoParams(FDele_update_interaction_widget_comp, UWidgetComponent*,FString)


// 전역 변수 / 함수 클래스
UCLASS()
class PUBG_UE4_API AGlobal : public AActor
{
	GENERATED_BODY()
		
private:
    static AGlobal* mp_global;

    // 플레이어 스프링암 관련
    UPROPERTY(VisibleAnywhere, Category = Manager)
        AData_table_manager* mp_data_table_manager;

    // UI 매니저
    //UPROPERTY(VisibleAnywhere, Category = Manager)
    //    AUI_manager* mp_UI_manager;

    // 사운드 매니저
    UPROPERTY(VisibleAnywhere, Category = Manager)
        ASound_manager* mp_sound_manager;

    UPROPERTY(VisibleAnywhere, Category = Game_instance)
        UCustom_game_instance* mp_game_instance;

public:
    FDele_update_interaction_widget_comp  dele_update_interaction_widget_comp;
	FRotator player_spring_arm_rotation;
	FVector  player_spring_arm_location;
	float	 player_spring_arm_length;

public:
    AGlobal();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float) override;

// 초기화 함수들
private:
	void Init();

	void Init_data_table_manager();

    void Init_game_instance();

    void Init_UI_manager();

    void Init_sound_manager();

    void Spawn_managers();

public:
    static void Set(AGlobal* _p_global) { if (_p_global) mp_global = _p_global; }

	static AGlobal* Get() { return mp_global; }

	static AData_table_manager* Get_data_table_manager();

	static UCustom_game_instance* Get_custom_game_inst();

    //static AUI_manager* Get_UI_manager();

    static ASound_manager* Get_sound_manager();
};