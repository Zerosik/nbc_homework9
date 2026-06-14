// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BNCGameModeBase.generated.h"

class ABNCPlayerController;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABNCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
public:
	virtual void Logout(AController* Exiting) override;
	
	// 플레이어 닉네임 입력시 접속자 관리
	void PlayerCustomLogin(AController* NewPlayer);
	
	// 채팅 메시지 처리
	void ProcessChattingMessage(ABNCPlayerController* InChattingPlayerController, const FString &InChatMessageString);
	
	void SendCommonChattingMessage(ABNCPlayerController* InPlayerController, const FString &InChatMessageString);
	
	void SendSystemChattingMessage(const FString &SystemMessageString, FColor InColor = FColor::Orange);
	void SendSystemNotificationMessage(const FString &SystemMessageString, FColor InColor = FColor::White);
	
	// 중복없는 숫자 3자리 맞냐?
	bool IsGuessingNumberString(const FString &InNumber);
	
	// PlayerState의 시도횟수 증가
	void IncreaseGuessCount(ABNCPlayerController* InChattingPlayerController);
	
	// PlayerState의 시도횟수 초기화
	void ResetGuessCount();
	
	// 최종게임판정
	void JudgeGame(ABNCPlayerController* InPlayerController, int32 StrikeCount);
	
	// 점수 확인
	FString JudgeResult(const FString& InGuessNumberString);
	
	//초기화. 시도횟수 날리고, 새 정답 만들고 등등
	void ResetGame();
	
	void PlayerAnswerTimeout();
	
private:
	void GenerateNewAnswer();
	
	int32 GetPlayerCount();
	
	bool bIsPlaying = false;
	
	FString Answer;
	
	TArray<TObjectPtr<ABNCPlayerController>> PlayerControllers;
	int32 CurrentPlayerIndex=0;
	
	FTimerHandle RestartTimerHandle;
	
	FTimerHandle AnswerTimerHandle;
};
