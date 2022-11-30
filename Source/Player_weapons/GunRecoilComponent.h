// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunRecoilComponent.generated.h"

class UCurveVector;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYER_WEAPONS_API UGunRecoilComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    APlayerController* PCRef = nullptr;

    // 필히 추가해야됨, 총기 반동 효과 줌
    UCurveVector* RecoilCurve;
    
    bool bRecoil;
    bool bFiring;    
    bool bRecoilRecovery;

    // 타이머 관련
    FTimerHandle FireTimer; // 반동 효과
    FTimerHandle RecoveryTimer; // 반동 후 제자리로 가는 효과

    float RecoveryTime = 1.f;
    float RecoverySpeed = 5.0f; // 원상 복구되는 시간

    float FireRate = 0.f;

    //Rotators
    FRotator RecoilStartRot; // 반동 시작 회전값
    FRotator RecoilDeltaRot; // 반동으로 인해 변경되는 회전값
    FRotator PlayerDeltaRot; // 플레이어 마우스 이동에 따라 변경되는 회전값
    
    FRotator pcRot;
    FRotator Del; // 틱 함수에서 사용 될 임시 변수

public:	
	// Sets default values for this component's properties
	UGunRecoilComponent();

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // 반동 주기 시작
    void RecoilStart();

    // 일정 시간 후 반동 멈춤
    void RecoilTimerFunction(); 

    // 반동 이전 원래 위치로 복구
    void RecoveryStart();

    // 일정 시간 후 원래 위치로 복구 방지
    void RecoveryTimerFunction() { bRecoilRecovery = false; }

    // 반동을 초당 주기 위해서 사용됨
    void RecoilTick(float DeltaTime);
};