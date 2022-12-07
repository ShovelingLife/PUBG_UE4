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
	// 이펙트 초기화
	void InitVfx();

	// 메쉬 초기화
	void InitMeshes();

	// 콜라이더 업데이트
	void UpdateCollider();

public:
	UFUNCTION()
	void TurnOffAfterLanding(class UPrimitiveComponent* HitComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};