#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Framework/Application/NavigationConfig.h"

void ACustomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 탭 키로 UI 내비게이션 비활성화
    if (IsLocalController())
    {
        TSharedRef<FNavigationConfig> p_navigation = MakeShared<FNavigationConfig>();
        p_navigation->bKeyNavigation    = false;
        p_navigation->bTabNavigation    = false;
        p_navigation->bAnalogNavigation = false;
        FSlateApplication::Get().SetNavigationConfig(p_navigation);
    }
}

void ACustomPlayerController::OnUnPossess()
{
    // 플레이어 컨트롤 바인딩 초기화
    if (auto p_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if(auto p_inputComp = p_playerController->InputComponent)
        {
            p_inputComp->ClearActionBindings();
            p_inputComp->AxisBindings.Empty();
        }
    }
}