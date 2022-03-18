#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameInstanceSubsystemUI.generated.h"

class AUI_manager;
class UItemSlotUI;
class AWeaponManager;

//DECLARE_DELEGATE_OneParam(FDeleSetTooltipData,)
DECLARE_DELEGATE_TwoParams(FDeleSetTooltipVisibility, UItemSlotUI*, ESlateVisibility)

UCLASS()
class UI_PUBG_API UGameInstanceSubsystemUI : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY() TSubclassOf< AUI_manager> UImanagerClass;

public:
    UPROPERTY() AUI_manager* pUImanager;
    FDeleSetTooltipVisibility DeleSetTooltipVisibility;

public:
	UGameInstanceSubsystemUI();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
    void InitUImanager();

public:
    /**
      * \brief 슬롯과 마우스간 뷰포트 기준 거리값을 구함
      * \return FVector2D 슬롯-마우스간 거리 값
     */
    FVector2D GetDistanceBetweenSlotCursor(class UUserWidget* _pWigdet, bool& _bFirst);

    bool IsMouseLeftFromUI(FVector2D _Distance, bool _bFirst = false);

    AWeaponManager* UGameInstanceSubsystemUI::GetWeaponManager();
};