#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyCharacter.generated.h"

class ABaseInteraction;

class USkeletalMeshComponent;
class USceneCaptureComponent2D;

UCLASS()
class CHARACTERS_API ADummyCharacter : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = RenderedActors)
	TArray<AActor*> mArrActorToShow;

public:
	UPROPERTY(EditAnywhere, Category = Mesh)		 
	USkeletalMeshComponent*   DummySkeletalMeshComp;

    UPROPERTY(EditAnywhere, Category = SceneCapture) 
	USceneCaptureComponent2D* SceneCaptureComp;

public:
	// Sets default values for this character's properties
	ADummyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// ------- 생성 관련 함수 -------
	void InitMeshComp();

	void InitAnimInstance();

    void InitRenderTarget();

    void InitWeaponUI();

    void UpdateWeapon();
};