#pragma once

#include "CoreMinimal.h"
#include "CoreFarmableItem.h"
#include "CrateBox.generated.h"

class UStaticMeshComponent;

UCLASS()
class FARMABLE_ITEMS_API ACrateBox : public ACoreFarmableItem
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Mesh) 
	UStaticMeshComponent* ParachuteMesh = nullptr;
        
    const float mkMaxParticleTime = 15.f;

	float mCurrentTime	  = 0.f;
	bool  mbParticleEnded = false;

public:
	ACrateBox();

protected:
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