// Fill out your copyright notice in the Description page of Project Settings.


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
        PlayerDeltaRot = FRotator(0.0f, 0.0f, 0.0f);
        RecoilDeltaRot = FRotator(0.0f, 0.0f, 0.0f);
        Del = FRotator(0.0f, 0.0f, 0.0f);
        RecoilStartRot = PCRef->GetControlRotation();

        bFiring = true;

        //Timer for the recoil: I have set it to 10s but dependeding how long it takes to empty the gun mag, you can increase the time.

        GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UGunRecoilComponent::RecoilTimerFunction, 10.0f, false);

        bRecoil = true;
        bRecoilRecovery = false;
    }
}

void UGunRecoilComponent::RecoilTimerFunction()
{
    bRecoil = false;
    GetWorld()->GetTimerManager().PauseTimer(FireTimer);
}

void UGunRecoilComponent::RecoveryStart()
{
    if (PCRef->GetControlRotation().Pitch > RecoilStartRot.Pitch)
    {
        bRecoilRecovery = true;
        GetWorld()->GetTimerManager().SetTimer(RecoveryTimer, this, &UGunRecoilComponent::RecoveryTimerFunction, RecoveryTime, false);
    }
}

void UGunRecoilComponent::RecoilTick(float DeltaTime)
{
    float recoiltime;
    FVector RecoilVec;

    if (bRecoil)
    {
        recoiltime = GetWorld()->GetTimerManager().GetTimerElapsed(FireTimer);
        RecoilVec = RecoilCurve->GetVectorValue(recoiltime);
        Del.Roll = 0;
        Del.Pitch = (RecoilVec.Y);
        Del.Yaw = (RecoilVec.Z);
        PlayerDeltaRot = PCRef->GetControlRotation() - RecoilStartRot - RecoilDeltaRot;
        PCRef->SetControlRotation(RecoilStartRot + PlayerDeltaRot + Del);
        RecoilDeltaRot = Del;

        if (!bFiring)
        {
            if (recoiltime > FireRate)
            {
                GetWorld()->GetTimerManager().ClearTimer(FireTimer);
                RecoilStop();
                bRecoil = false;
                RecoveryStart();

            }
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
            RecoveryTimer.Invalidate();
    }
}