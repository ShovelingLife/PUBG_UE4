// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyCharacter.generated.h"

UCLASS()
class CHARACTERS_API ADummyCharacter : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Mesh)		 class USkeletalMeshComponent*	 SkeletalMeshComp;
	UPROPERTY(EditAnywhere, Category = SceneCapture) class USceneCaptureComponent2D* SceneCaptureComp;

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
	void InitMeshComp();

	void InitAnimInstance();

	void InitRenderTarget();
};