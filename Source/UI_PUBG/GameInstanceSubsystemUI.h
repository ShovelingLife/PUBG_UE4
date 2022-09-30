#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameInstanceSubsystemUI.generated.h"

class AUI_manager;
class ACoreAttachment;
class AInventoryManager;
class AWeaponManager;
class UItemSlotUI;

//DECLARE_DELEGATE_OneParam(FDeleSetTooltipData,)
DECLARE_DELEGATE_OneParam(FDeleVerifyAttachmentSlot, ACoreAttachment*)
DECLARE_DELEGATE_RetVal_OneParam(UItemSlotUI*, FDeleActivateCursorSlot, bool)
DECLARE_DELEGATE_TwoParams(FDeleSetTooltipVisibility, UItemSlotUI*, ESlateVisibility)

UCLASS()
class UI_PUBG_API UGameInstanceSubsystemUI : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
    static UWorld* mpWorld;
    UPROPERTY() TSubclassOf<AUI_manager> mUImanagerClass;

public:
    UPROPERTY() AUI_manager* pUImanager;
    FDeleSetTooltipVisibility DeleSetTooltipVisibility;
    FDeleVerifyAttachmentSlot DeleVerifyAttachmentSlot;
    FDeleActivateCursorSlot DeleActivateCursorSlot;
    FDelegateHandle TickDelegateHandle;

public:
	UGameInstanceSubsystemUI();

public:
    static UGameInstanceSubsystemUI* GetInst();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    virtual void Deinitialize() override;

    bool Tick(float DeltaSeconds);

private:
    void InitUImanager();


public:
    /**
      * \brief ���԰� ���콺�� ����Ʈ ���� �Ÿ����� ����
      * \return FVector2D ����-���콺�� �Ÿ� ��
     */
    FVector2D GetDistanceBetweenSlotCursor(class UUserWidget* pWigdet, bool& bFirst);

    bool IsMouseLeftFromUI(FVector2D Distance, bool bFirst = false);

    AWeaponManager* GetWeaponManager();

    AInventoryManager* GetInventoryManager();
};