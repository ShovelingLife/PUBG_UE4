#include "CursorSlotUI.h"
#include <Blueprint/SlateBlueprintLibrary.h>
#include <Kismet/KismetMathLibrary.h>

void UCursorSlotUI::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
    auto geometry = this->GetCachedGeometry();

    //GEngine->AddOnScreenDebugMessage(6, 1.f, FColor::Cyan, geometry.po);
    //GEngine->AddOnScreenDebugMessage(7, 1.f, FColor::Red, FString::Printf(TEXT("Clicked pos : %s, Current pos : %s"), *ClickPos.ToString(),
    //    *USlateBlueprintLibrary::AbsoluteToLocal(geometry, geometry.GetAbsolutePosition()).ToString()));
}