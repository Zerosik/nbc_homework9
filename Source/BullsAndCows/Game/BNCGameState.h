// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BNCGameState.generated.h"

/**
 * 
 */

UCLASS()
class BULLSANDCOWS_API ABNCGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABNCGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;
	float GetTimeoutLeft();
	void SetTimeout(float NextTimeout = 10);
	
	UPROPERTY(Replicated)
	float TimeElapsed = 0.f;
	
	UPROPERTY(Replicated)
	float TimeLimit = 0.f ;
	
};
