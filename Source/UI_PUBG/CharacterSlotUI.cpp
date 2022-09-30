#include "CharacterSlotUI.h"
#include "CustomDragDropOperation.h"
#include "GameInstanceSubsystemUI.h"
#include "ItemSlotUI.h"
#include "Characters/CustomPlayer.h"
#include "Characters/DummyCharacter.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UCharacterSlotUI::NativeConstruct()
{
    Super::NativeConstruct();
    InitSlotUI();
}

void UCharacterSlotUI::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);
    UpdateHighlightImg();
}

FReply UCharacterSlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // 왼쪽 클릭
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        if (CharacterImg->IsHovered())
            mCharacterUIClickPos = InMouseEvent.GetScreenSpacePosition();
    }
    auto reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return reply.NativeReply;
}

FReply UCharacterSlotUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    mCharacterUIClickPos = FVector2D::ZeroVector;   
    return FReply::Handled();
}

FReply UCharacterSlotUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseMove(InGeometry, InMouseEvent);
    return FReply::Handled();
}

void UCharacterSlotUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    auto p_customDragDropOperation = NewObject<UCustomDragDropOperation>();
    OutOperation = p_customDragDropOperation;
}

bool UCharacterSlotUI::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

    if (auto p_customDragOp = Cast<UCustomDragDropOperation>(InOperation))
    {
        if (auto p_slot = p_customDragOp->GetSlot())
        {
            if (auto pGameInstanceSubsystemUI = UGameInstance::GetSubsystem<UGameInstanceSubsystemUI>(GetWorld()->GetGameInstance()))
                pGameInstanceSubsystemUI->DeleVerifyAttachmentSlot.ExecuteIfBound(nullptr);
        }
    }
    // 캐릭터 UI 창 드래그 시 회전
    if (mCharacterUIClickPos != FVector2D::ZeroVector)
    {
        auto currentMousePos  = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());        
        auto p_dummyCharacter = Cast<ACustomPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->pDummyCharacter;
        
        if (p_dummyCharacter)
        {
            FVector2D trackPos       = mCharacterUIClickPos - currentMousePos;
            float     rotationValInZ = (trackPos.X - (trackPos.Y + 140.f)) * 0.1;
            
            // 회전 제한 속도 설정
            if (rotationValInZ > 5.f)
                rotationValInZ = 5.f;

            if (rotationValInZ < -5.f)
                rotationValInZ = -5.f;

            p_dummyCharacter->DummySkeletalMeshComp->AddWorldRotation(FRotator::MakeFromEuler(FVector(0.f, 0.f, rotationValInZ)));
        }      
    }
    return true;
}

void UCharacterSlotUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
    mCharacterUIClickPos = FVector2D::ZeroVector;
}

bool UCharacterSlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    mCharacterUIClickPos = FVector2D::ZeroVector;
    return true;
}

void UCharacterSlotUI::InitSlotUI()
{
    TArray< UItemSlotUI* > p_arrSlotUI
    {
        // 장비 관련
        HeadSlotUI,
        BodyArmorSlotUI,
        BackpackSlotUI,
        BeltSlotUI,

        // 스킨 관련
        HatSlotUI,
        MaskSlotUI,
        CoatSlotUI,
        ShirtSlotUI,
        GlovesSlotUI,
        PantsSlotUI,
        ShoesSlotUI
    };
    for(const auto item : p_arrSlotUI)
        item->BackgroundSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterSlotUI::UpdateHighlightImg()
{
    UBorder* tmpBorder = nullptr;
    TArray<UBorder*> p_ArrSlot
    {
        HeadSlotBorder,
        BodyArmorSlotBorder,
        BackpackSlotBorder,
        BeltSlotBorder,
        HatSlotBorder,
        MaskSlotBorder,
        CoatSlotBorder,
        ShirtSlotBorder,
        GlovesSlotBorder,
        PantsSlotBorder,
        ShoesSlotBorder 
    };
    // 11번 순회
    for (const auto item : p_ArrSlot)
    {
        // 칸이 선택 되었을 시
    	if (item->IsHovered())
        {
            GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, "Hovered");
            tmpBorder = item;
            break;
        }
    }
    // 선택된게 있다면 하얀 칸으로 표시
    if (tmpBorder)
    {
        FVector2D dummyPos, movePos;
        auto geometry = tmpBorder->GetCachedGeometry();
        USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), geometry.GetAbsolutePosition(), dummyPos, movePos);
        movePos.X = (movePos.X <= 818.f) ? -435.f : 50.f;
        movePos.Y += 1.f;
        //GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, geometry.GetAbsolutePosition().ToString() + " " + movePos.ToString());

        if (auto p_canvasSlot = Cast<UCanvasPanelSlot>(HighlightImg->Slot))
            p_canvasSlot->SetPosition(movePos);
    }
    HighlightImg->SetVisibility((tmpBorder) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}