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

    // ���� �߰��ؾߵ�, �ѱ� �ݵ� ȿ�� ��
    UCurveVector* RecoilCurve;
    
    bool bRecoil;
    bool bFiring;    
    bool bRecoilRecovery;

    // Ÿ�̸� ����
    FTimerHandle FireTimer; // �ݵ� ȿ��
    FTimerHandle RecoveryTimer; // �ݵ� �� ���ڸ��� ���� ȿ��

    float RecoveryTime = 1.f;
    float RecoverySpeed = 5.0f; // ���� �����Ǵ� �ð�

    float FireRate = 0.f;

    //Rotators
    FRotator RecoilStartRot; // �ݵ� ���� ȸ����
    FRotator RecoilDeltaRot; // �ݵ����� ���� ����Ǵ� ȸ����
    FRotator PlayerDeltaRot; // �÷��̾� ���콺 �̵��� ���� ����Ǵ� ȸ����
    
    FRotator pcRot;
    FRotator Del; // ƽ �Լ����� ��� �� �ӽ� ����

public:	
	// Sets default values for this component's properties
	UGunRecoilComponent();

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // �ݵ� �ֱ� ����
    void RecoilStart();

    // ���� �ð� �� �ݵ� ����
    void RecoilTimerFunction(); 

    // �ݵ� ���� ���� ��ġ�� ����
    void RecoveryStart();

    // ���� �ð� �� ���� ��ġ�� ���� ����
    void RecoveryTimerFunction() { bRecoilRecovery = false; }

    // �ݵ��� �ʴ� �ֱ� ���ؼ� ����
    void RecoilTick(float DeltaTime);
};