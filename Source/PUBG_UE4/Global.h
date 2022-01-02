// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global.generated.h"

// 전역 변수 / 함수 클래스
UCLASS()
class PUBG_UE4_API AGlobal : public AActor
{
	GENERATED_BODY()
	
public:	
	// 플레이어 스프링암 관련
	static float	player_spring_arm_length;
	static FRotator player_spring_arm_rotation;
	static FVector  player_spring_arm_location;
};