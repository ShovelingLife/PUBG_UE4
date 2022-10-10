#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PUBG_UE4/WeaponEnum.h"
#include "WeaponSocketUI.generated.h"

class UBorder;
class UImage;

UCLASS()
class UI_PUBG_API UWeaponSocketUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UBorder* ItemBorder;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage*  ItemImg;
    class ACoreAttachment* Attachment = nullptr;
    EAttachmentType AttachmentType;
    int WeaponIdx = 1;

public:
    // USocketUI(const FObjectInitializer& ObjectInitializer);

public:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;
};