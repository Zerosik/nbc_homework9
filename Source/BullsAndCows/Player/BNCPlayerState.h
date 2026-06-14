// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BNCPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABNCPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABNCPlayerState();
	
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetPlayerNickname(const FString& NewNickname);
	FString& GetPlayerNickname();
	int32 GetCurrentGuessCount() const;
	int32 GetMaxGuessCount() const;
	void SetCurrentGuessCount(int32 CurrentGuessCount);
	void SetMaxGuessCount(int32 MaxGuessCount);

private:
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
	UPROPERTY(Replicated)
	FString PlayerNickName;
};
