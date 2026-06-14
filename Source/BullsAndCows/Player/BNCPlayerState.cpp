// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BNCPlayerState.h"
#include "Net/UnrealNetwork.h"

ABNCPlayerState::ABNCPlayerState(): 
CurrentGuessCount(0),
MaxGuessCount(3),
PlayerNickName()
{
	bReplicates = true;
}

void ABNCPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ABNCPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
	DOREPLIFETIME(ThisClass, PlayerNickName);
}

void ABNCPlayerState::SetPlayerNickname(const FString& NewNickname)
{
	PlayerNickName = NewNickname;
}

FString& ABNCPlayerState::GetPlayerNickname()
{
	return PlayerNickName;
}

int32 ABNCPlayerState::GetCurrentGuessCount() const
{
	return CurrentGuessCount;
}

int32 ABNCPlayerState::GetMaxGuessCount() const
{
	return MaxGuessCount;
}

void ABNCPlayerState::SetCurrentGuessCount(int32 NewGuessCount)
{
	this->CurrentGuessCount = NewGuessCount;
}

void ABNCPlayerState::SetMaxGuessCount(int32 NewGuessCount)
{
	this->MaxGuessCount = NewGuessCount;
}

