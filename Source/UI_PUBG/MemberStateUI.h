#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MemberStateUI.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class UI_PUBG_API UMemberStateUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
// ------- 1번 플레이어 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* NameTxt1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UProgressBar* HP_bar1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* StateImg1;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* PingImg1;

// ------- 2번 플레이어 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* NameTxt2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UProgressBar* HP_bar2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* StateImg2;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* PingImg2;

// ------- 3번 플레이어 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* NameTxt3;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UProgressBar* HP_bar3;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* StateImg3;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* PingImg3;

// ------- 4번 플레이어 -------

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UTextBlock* NameTxt4;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UProgressBar* HP_bar4;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* StateImg4;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
    UImage* PingImg4;

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry&, float) override;
};