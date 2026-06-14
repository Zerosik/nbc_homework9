// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BNCGameState.h"
#include "Net/UnrealNetwork.h"


ABNCGameState::ABNCGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABNCGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TimeElapsed);
	DOREPLIFETIME(ThisClass, TimeLimit);
}

void ABNCGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TimeElapsed += DeltaSeconds;
}

float ABNCGameState::GetTimeoutLeft()
{
	float TimeLeft = FMath::Clamp(TimeLimit - TimeElapsed, 0, 10);
	//UE_LOG(LogTemp, Warning, TEXT("TimeLimit = %f"), TimeLeft);
	return TimeLeft;
}

void ABNCGameState::SetTimeout(float NextTimeout)
{
	TimeElapsed = 0;
	TimeLimit = NextTimeout;
}
