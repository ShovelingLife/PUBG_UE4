﻿#include "CharacterSlotUI.h"
#include "CustomDragDropOperation.h"
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
    
    // 캐릭터 UI 창 드래그 시 회전
    if (mCharacterUIClickPos != FVector2D::ZeroVector)
    {
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

            p_dummyCharacter->DummySkeletalMeshComp->AddRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, 0.f, rotationValInZ)));
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
    TArray<Chaos::Pair<UBorder*, UImage*>> p_ArrSlot
    {
        Chaos::MakePair< UBorder*, UImage*>(HeadSlotBorder,      HeadSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(BodyArmorSlotBorder, BodyArmorSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(BackpackSlotBorder,  BackpackSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(BeltSlotBorder,      BeltSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(HatSlotBorder,       HatSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(MaskSlotBorder,      MaskSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(CoatSlotBorder,      CoatSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(ShirtSlotBorder,     ShirtSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(GlovesSlotBorder,    GlovesSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(PantsSlotBorder,     PantsSlotImg),
        Chaos::MakePair< UBorder*, UImage*>(ShoesSlotBorder,     ShoesSlotImg)
    };
    for (const auto item : p_ArrSlot)
    {
        // 선택 되었을 시
    	if (item.Second->IsHovered())
        {
            tmpBorder = item.First;
            break;
        }
    }
    if (tmpBorder)
    {
        FVector2D dummyVector, movePos;
        USlateBlueprintLibrary::LocalToViewport(GetWorld(), tmpBorder->GetCachedGeometry(), FVector2D::ZeroVector, dummyVector, movePos);

        if (movePos.X == 580)
            movePos.X = 0;

        if (movePos.X == 1231)
            movePos.X /= 1.89f;
        
        if (auto p_canvasSlot = Cast<UCanvasPanelSlot>(HighlightImg->Slot))
        {
            p_canvasSlot->SetPosition(movePos);
            HighlightImg->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
        HighlightImg->SetVisibility(ESlateVisibility::Hidden);
}