#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Member_State_UI.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class UI_PUBG_API UMember_State_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 1번 플레이어
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Name_player_1_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* HP_player_1_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_1_state_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_1_ping_img;

    // 2번 플레이어
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Name_player_2_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* HP_player_2_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_2_state_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_2_ping_img;

    // 3번 플레이어
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Name_player_3_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* HP_player_3_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_3_state_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_3_ping_img;

    // 4번 플레이어
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock*   Name_player_4_text;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UProgressBar* HP_player_4_bar;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_4_state_img;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*       Player_4_ping_img;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};