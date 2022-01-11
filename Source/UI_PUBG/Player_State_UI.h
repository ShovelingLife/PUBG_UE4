#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player_State_UI.generated.h"

UCLASS()
class UI_PUBG_API UPlayer_State_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // ü�� ȸ����(�������帵ũ, ��������) ���
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* HP_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Recovery_bar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UProgressBar* Oxygen_bar;

    // ��� ���� ���� �̹��� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Backbag_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Head_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Bodyarmor_img;

    // ���� ���� ���� �̹��� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Speedup_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Healing_img;

    // ������� �ѱ� ��Ȳ
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_current_text;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_total_text;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* BoltAction_img;

    // �ڼ�
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Posture_img;

    // ���ؼ� �̹���
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Reticle_img;

    class ACustom_player* p_custom_player;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;

public:
    // ���� ���� ����
    void Update_weapon_slot_UI();

    // ���ؼ� ����
    void Update_aim_UI();

    // �Ѿ� ���� ����
    void Update_bullet_count_UI();

    // ��� �� ����
    void Update_oxygen_bar_UI(float);

    void Change_shoot_mode();
};
