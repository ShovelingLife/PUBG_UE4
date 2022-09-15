#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SocketUI.generated.h"

class UBorder;
class UImage;

UCLASS()
class UI_PUBG_API USocketUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* ItemBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  ItemImg;
    class ACoreAttachment* Attachment = nullptr;

public:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;
};