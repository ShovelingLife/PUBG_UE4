#include "CharacterSlotUI.h"
#include "CustomDragDropOperation.h"
#include "Characters/CustomPlayer.h"
#include "Characters/DummyCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

}

void UCharacterSlotUI::NativeTick(const FGeometry& _MyGeometry, float _DeltaTime)
{
    Super::NativeTick(_MyGeometry, _DeltaTime);
}

FReply UCharacterSlotUI::NativeOnMouseButtonDown(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseButtonDown(_InGeometry, _InMouseEvent);

    // 왼쪽 클릭
    if (_InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        if (CharacterImg->IsHovered())
            mCharacterUIClickPos = _InMouseEvent.GetScreenSpacePosition();
    }

    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(_InMouseEvent, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UCharacterSlotUI::NativeOnMouseButtonUp(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseButtonUp(_InGeometry, _InMouseEvent);
    mCharacterUIClickPos = FVector2D::ZeroVector;   
    return FReply::Handled();
}

FReply UCharacterSlotUI::NativeOnMouseMove(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent)
{
    Super::NativeOnMouseMove(_InGeometry, _InMouseEvent);
    return FReply::Handled();
}

void UCharacterSlotUI::NativeOnDragDetected(const FGeometry& _InGeometry, const FPointerEvent& _InMouseEvent, UDragDropOperation*& _OutOperation)
{
    Super::NativeOnDragDetected(_InGeometry, _InMouseEvent, _OutOperation);
    auto p_customDragDropOperation = NewObject<UCustomDragDropOperation>();
    _OutOperation = p_customDragDropOperation;
}

bool UCharacterSlotUI::NativeOnDragOver(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDragOver(_InGeometry, _InDragDropEvent, _InOperation);
    
    // 캐릭터 UI 창 드래그 시 회전
    if (mCharacterUIClickPos != FVector2D::ZeroVector)
    {
        //auto currentMousePos = _InMouseEvent.GetScreenSpacePosition();
        auto currentMousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
        
        ADummyCharacter* p_dummyCharacter = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->pDummyCharacter;
        
        if (p_dummyCharacter)
        {
            FVector2D trackPos       = mCharacterUIClickPos - currentMousePos;
            float     rotationValInZ = (trackPos.X - (trackPos.Y + 140.f)) * 0.1;

            // 회전 제한 속도 설정
            if (rotationValInZ > 5.f)
                rotationValInZ = 5.f;

            if (rotationValInZ < -5.f)
                rotationValInZ = -5.f;

            p_dummyCharacter->SkeletalMeshComp->AddRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, 0.f, rotationValInZ)));
            GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, trackPos.ToString());
        }      
    }
    return true;
}

void UCharacterSlotUI::NativeOnDragCancelled(const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDragCancelled(_InDragDropEvent, _InOperation);
    mCharacterUIClickPos = FVector2D::ZeroVector;
}

bool UCharacterSlotUI::NativeOnDrop(const FGeometry& _InGeometry, const FDragDropEvent& _InDragDropEvent, UDragDropOperation* _InOperation)
{
    Super::NativeOnDrop(_InGeometry, _InDragDropEvent, _InOperation);
    mCharacterUIClickPos = FVector2D::ZeroVector;
    return true;
}