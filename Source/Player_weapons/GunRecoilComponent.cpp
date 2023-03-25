#include "GunRecoilComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveVector.h"

// Sets default values for this component's properties
UGunRecoilComponent::UGunRecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGunRecoilComponent::BeginPlay()
{
    Super::BeginPlay();
    PCRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called every frame
void UGunRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RecoilTick(DeltaTime);
}

void UGunRecoilComponent::RecoilStart()
{   
    if (RecoilCurve)
    {
        PlayerDeltaRot = RecoilDeltaRot = Del = FRotator::ZeroRotator;
        RecoilStartRot = PCRef->GetControlRotation();
       
        // 반동 효과 주기위한 (10초 후)
        GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UGunRecoilComponent::RecoilTimer, 10.0f, false);

        bFiring = bRecoil = true;
        bRecoilRecovery = false;
    }
}

void UGunRecoilComponent::RecoilTimer()
{
    bRecoil = false;
    GetWorld()->GetTimerManager().PauseTimer(FireTimerHandle);
}

void UGunRecoilComponent::RecoveryStart()
{
    if (PCRef->GetControlRotation().Pitch > RecoilStartRot.Pitch)
    {
        bRecoilRecovery = true;
        GetWorld()->GetTimerManager().SetTimer(RecoveryTimerHandle, this, &UGunRecoilComponent::RecoveryTimer, RecoveryTime, false);
    }
}

void UGunRecoilComponent::RecoilTick(float DeltaTime)
{
    FVector RecoilVec;
    float recoiltime;

    if (bRecoil)
    {
        recoiltime = GetWorld()->GetTimerManager().GetTimerElapsed(FireTimerHandle);
        RecoilVec = RecoilCurve->GetVectorValue(recoiltime);
        Del = FRotator(RecoilVec.Y, RecoilVec.Z, 0.f);
        PlayerDeltaRot = PCRef->GetControlRotation() - RecoilStartRot - RecoilDeltaRot;
        PCRef->SetControlRotation(RecoilStartRot + PlayerDeltaRot + Del);
        RecoilDeltaRot = Del;

        // 반동 멈춤
        if (!bFiring &&
            recoiltime > FireRate)
        {
            GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
            RecoveryStart();
            bFiring = false;
            bRecoil = false;
        }
    }
    else if (bRecoilRecovery)
    {
        // 반동 초기화
        FRotator tmpRot = PCRef->GetControlRotation();
        pcRot = tmpRot;

        if (tmpRot.Pitch >= RecoilStartRot.Pitch)
        {
            PCRef->SetControlRotation(UKismetMathLibrary::RInterpTo(pcRot, pcRot - RecoilDeltaRot, DeltaTime, 10.0f));
            RecoilDeltaRot = RecoilDeltaRot + (pcRot - tmpRot);
        }
        else
            RecoveryTimerHandle.Invalidate();
    }
}