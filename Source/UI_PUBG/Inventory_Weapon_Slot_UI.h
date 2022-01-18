#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Inventory_Weapon_Slot_UI.generated.h"

UCLASS()
class UI_PUBG_API UInventory_Weapon_Slot_UI : public UUserWidget
{
	GENERATED_BODY()
	

public:
    //1�� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_1_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type_1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_1_txt;

    //2�� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_2_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type_2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_2_txt;

    //3�� ����
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_3_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type_3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine_3_txt;

    //4�� ���� (��������)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_4_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_4_txt;

    //5�� ���� (��ô����)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot_5_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name_5_txt;

    // �κ��丮 UI
    UPROPERTY(VisibleAnywhere, Category = Weapon_UI)
        TMap<int, UTexture*> map_weapon_ui_tex;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;
};