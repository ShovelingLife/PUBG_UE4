#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlotItemData.h"
#include "PUBG_UE4/MyEnum.h"
#include "PUBG_UE4/WeaponData.h"
#include "PUBG_UE4/OtherWeaponData.h"
#include "UI_manager.generated.h"

class UPlayerUI;
class UPlayerEffectUI;
class UCursorSlotUI;
class UItemSlotUI;
class AInventoryManager;
class UTooltipUI;

UCLASS()
class UI_PUBG_API AUI_manager : public AActor
{
	GENERATED_BODY()

using MatMap = TMap<int, UMaterial*>;
using TexMap = TMap<int, UTexture*>;

public:
    static TexMap MapPlayerIcon;           // 플레이어 UI

    // 무기 UI 관련 (인벤토리)
    UPROPERTY(EditAnywhere) 
    TArray< UTexture*> ArrMainWeaponIcon;

    static MatMap MapMainWeaponIcon;       // 메인 무기 UI 벡터 이미지  
    static TexMap MapWeaponIcon;           // 인벤토리 무기 UI (총기)   
    static TexMap MapOtherWeaponIcon;      // 인벤토리 무기 UI (근접 및 투척류)
    static TexMap MapWeaponAttachmentIcon; // 부속품 UI

    // 그 외 UI 관련
    static TexMap MapAmmoBoxIcon; // 탄박스 UI
    static TexMap MapUtilityIcon; // 그 외 상호작용 가능한 오브젝트 UI (가방, 회복아이템 등)

private:
    // ------- 현재 쓰이고있는 변수들 -------

    // 플레이어 UI 
    TSubclassOf<UPlayerUI> mBP_PlayerUI;

    UPROPERTY() 
    UPlayerUI* mpPlayer_UI;

    // 플레이어 상태이상 UI
    TSubclassOf<UPlayerEffectUI> mBP_PlayerEffectUI;

    // 상호작용 UI 관련 
    UPROPERTY(VisibleAnywhere, Category = WidgetBP) 
    TSubclassOf<UUserWidget> mInteractionWidgetBP;

    // 메인 플레이어 UI 리소스 경로명
    const TArray<FString> mkArrShootType 
    {
        "SingleShot",
        "BurstShot",
        "ConsecutiveShot",
    };

public:
    UPROPERTY() 
    AInventoryManager* pInventoryManager;

    UPROPERTY() 
    UPlayerEffectUI* pPlayerEffect_UI;

public:
    AUI_manager();

public:
    static UTexture2D* GetTexture2D(FsSlotItemData ItemData);

    static UTexture2D* GetTexture2D(int Idx, FString Type = "");

    static UMaterial* GetMaterial(int Idx);

protected:
    virtual void BeginPlay() override;

private:
// ------- 초기화 관련 -------

    void BindDelegate();

    // 상호작용 UI 초기화
    void InitInteractionUI();

    // 최상위 플레이어 UI 초기화
    void InitPlayerUI();

    // 플레이어 상태이상 UI 초기화
    void InitPlayerEffectUI();

// ------- 아이콘 UI 관련 -------

    // 플레이어 UI 초기화
    void InitPlayerIcon();

    // 플레이어 무기 선택 UI 초기화
    void InitMainWeaponIcon();

    // 인벤토리 무기 UI 초기화
    void InitInventoryWeaponIcon();

    // 기타 UI 초기화
    void InitUtilityIcon();

// ------- 시작 후 초기화 -------
public:
    // 플레이어 인벤토리 설정
    void InitPlayerInventory();

    // 플레이어 UI 띄움
    void SetPlayerUI();

    // 미구현 상태
    void SetWeaponUI();

public:
    // 상호작용 UI 업데이트 (델리게이트)
    UFUNCTION() 
    void UpdateInteractionUI(class UWidgetComponent* WidgetComp, FString Title);

    UFUNCTION() 
    void RunEffectAnim(float StartTime, float WaitTime, EPlayerStateAnimType Type);

    UFUNCTION() 
    void KillAnim();
};