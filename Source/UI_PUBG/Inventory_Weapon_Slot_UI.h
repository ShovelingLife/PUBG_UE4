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
	
private:
    class AUI_manager*     mp_UI_manager     = nullptr;
    class AWeapon_manager* mp_weapon_manager = nullptr;

public:
    //1¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot1_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine1_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Number1_txt;

    //2¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot2_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine2_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Number2_txt;

    //3¹ø ½½·Ô
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot3_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Bullet_type3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Magazine3_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Number3_txt;

    //4¹ø ½½·Ô (±ÙÁ¢¹«±â)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot4_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name4_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Number4_txt;

    //5¹ø ½½·Ô (ÅõÃ´¹«±â)
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* Gun_slot5_img;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Gun_name5_txt;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* Number5_txt;

    // ÀÎº¥Åä¸® UI
    UPROPERTY(VisibleAnywhere, Category = Weapon_UI)
        TMap<int, UTexture*> map_weapon_ui_tex;

protected:
    void NativeConstruct() override;

    void NativeTick(const FGeometry&, float) override;

private:
    void Update_UI_visibility();

    void Update_inventory_weapon_UI();
};