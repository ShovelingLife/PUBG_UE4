#include "Custom_player_controller.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Framework/Application/NavigationConfig.h"

void ACustom_player_controller::BeginPlay()
{
    Super::BeginPlay();

    // 탭 키로 UI 내비게이션 비활성화
    if(IsLocalController())
    {
        TSharedRef<FNavigationConfig> p_navigation = MakeShared<FNavigationConfig>();
        p_navigation->bKeyNavigation    = false;
        p_navigation->bTabNavigation    = false;
        p_navigation->bAnalogNavigation = false;
        FSlateApplication::Get().SetNavigationConfig(p_navigation);
    }
}

void ACustom_player_controller::OnUnPossess()
{
    // 플레이어 컨트롤 바인딩 초기화
    auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (!player_controller)
    {
        GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, "Player_controller_null");
        return;
    }
    auto input_component = player_controller->InputComponent;

    if (!input_component)
    {
        GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, "Input_component_null");
        return;
    }
    input_component->ClearActionBindings();
    input_component->AxisBindings.Empty();
    //GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Red, "Cleared");
}