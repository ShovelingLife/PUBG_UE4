#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "CrateBox.generated.h"

/**
 * 
 */
UCLASS()
class FARMABLE_ITEMS_API ACrateBox : public ACoreFarmableItem
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Mesh) class UStaticMeshComponent* ParachuteMesh = nullptr;
        
	float mCurrentTime			  = 0.f;
    const float mkMaxParticleTime = 15.f;
	bool  mbParticleEnded		  = false;

public:
	ACrateBox();

protected:
    // Called every frame
    virtual void Tick(float) override;

private:
	// ����Ʈ �ʱ�ȭ
	void InitVfx();

	// �޽� �ʱ�ȭ
	void InitMeshes();

	// �ݶ��̴� ������Ʈ
	void UpdateCollider();

public:
	UFUNCTION()
	void TurnOffAfterLanding(class UPrimitiveComponent* HitComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};