// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BNCGameModeBase.h"

#include "BNCGameState.h"
#include "Algo/RandomShuffle.h"
#include "Player/BNCPlayerController.h"
#include "Player/BNCPlayerState.h"

void ABNCGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GenerateNewAnswer();
}

void ABNCGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABNCGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	
}

void ABNCGameModeBase::ForEachPlayerController(TFunctionRef<void(ABNCPlayerController*)> Callback) const
{
	for (ABNCPlayerController* BNCPlayerController : PlayerControllers)
	{
		if (IsValid(BNCPlayerController))
		{
			Callback(BNCPlayerController);
		}
	}
}

bool ABNCGameModeBase::AreAllPlayersOutOfGuesses() const
{
	bool bAllPlayersOutOfGuesses = true;
	ForEachPlayerController([&bAllPlayersOutOfGuesses](ABNCPlayerController* BNCPlayerController)
	{
		ABNCPlayerState* BNCPS = BNCPlayerController->GetPlayerState<ABNCPlayerState>();
		if (IsValid(BNCPS) && BNCPS->GetCurrentGuessCount() < BNCPS->GetMaxGuessCount())
		{
			bAllPlayersOutOfGuesses = false;
		}
	});
	return bAllPlayersOutOfGuesses;
}

void ABNCGameModeBase::HandleDrawGame()
{
	bIsPlaying = false;
	FString DrawNotificationText = FString::Printf(TEXT("무승부"));
	FString SystemMessage = FString::Printf(TEXT("정답은 %s이었습니다. 5초후 게임을 재시작합니다"), *Answer);
			
	SendSystemNotificationMessage(DrawNotificationText, FColor::Orange);
	SendSystemChattingMessage(SystemMessage);
	StartAnswerTimer(0);
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ABNCGameModeBase::ResetGame, 5, false);
	GetWorldTimerManager().ClearTimer(AnswerTimerHandle);
}

void ABNCGameModeBase::AdvanceCurrentPlayer()
{
	if (PlayerControllers.IsEmpty())
	{
		CurrentPlayerIndex = 0;
		return;
	}
	
	CurrentPlayerIndex++;
	if (CurrentPlayerIndex >= PlayerControllers.Num())
	{
		CurrentPlayerIndex = 0;
	}
}


void ABNCGameModeBase::StartAnswerTimer(float TimeLimit)
{
	if (ABNCGameState* BNCGS = GetGameState<ABNCGameState>())
	{
		BNCGS->SetTimeout(TimeLimit);
	}
	// TimeLimit을 0으로 호출시 타이머 UI를 정리할수있도록 함.
	if (TimeLimit>0)
		GetWorldTimerManager().SetTimer(AnswerTimerHandle, this, &ABNCGameModeBase::PlayerAnswerTimeout, 10, false);
}

void ABNCGameModeBase::NotifyCurrentTurnPlayer()
{
	if (PlayerControllers.IsValidIndex(CurrentPlayerIndex) == false)
		return;
	
	ABNCPlayerController* BNCPlayerController = PlayerControllers[CurrentPlayerIndex];
	if (IsValid(BNCPlayerController) == false)
		return;
	
	ABNCPlayerState* BNCPS = BNCPlayerController->GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS) == false)
		return;
	
	SendSystemNotificationMessage(FString::Printf(TEXT("%s의 턴!"), *BNCPS->GetPlayerNickname()), FColor::Yellow);
}

void ABNCGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ABNCPlayerController* BNCPlayerController = Cast<ABNCPlayerController>(Exiting);
	if (IsValid(BNCPlayerController) == false)
		return;
	
	ABNCPlayerState* BNCPS = BNCPlayerController->GetPlayerState<ABNCPlayerState>();
	const FString Nickname = IsValid(BNCPS) ? BNCPS->GetPlayerNickname() : FString();
	PlayerControllers.Remove(BNCPlayerController);
	
	if (PlayerControllers.Num()<=1)
	{
		FString GameDelayString = FString::Printf(TEXT("플레이어가 2명 미만이 되어 대기합니다."));
		SendSystemNotificationMessage(FString::Printf(TEXT("대기중...")));
		SendSystemChattingMessage(GameDelayString);
		bIsPlaying = false;
		GetWorldTimerManager().ClearTimer(AnswerTimerHandle);
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
		ABNCGameState* BNCGS = GetGameState<ABNCGameState>();
		if (IsValid(BNCGS))
		{
			BNCGS->SetTimeout(0);
		}
	}else
	{
		FString GameDelayString = FString::Printf(TEXT("%s가 퇴장하여 게임을 재시작합니다"), *Nickname);
		SendSystemNotificationMessage(FString::Printf(TEXT("재시작중...")));
		SendSystemChattingMessage(GameDelayString);
		bIsPlaying = false;
		GetWorldTimerManager().ClearTimer(AnswerTimerHandle);
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ABNCGameModeBase::ResetGame, 3, false);
		StartAnswerTimer(0);
		
	}
	
}

void ABNCGameModeBase::PlayerCustomLogin(AController* NewPlayer)
{
	ABNCPlayerController* BNCPlayerController = Cast<ABNCPlayerController>(NewPlayer);
	if (IsValid(BNCPlayerController) == false)
		return;
	
	ABNCPlayerState* BNCPS = BNCPlayerController->GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS) == false)
		return;
	
	PlayerControllers.AddUnique(BNCPlayerController);
	FString PlayerJoiningMessage = FString::Printf(TEXT("플레이어 %s가 접속하였습니다"), *BNCPS->GetPlayerNickname());
	
	SendCommonChattingMessage(PlayerJoiningMessage);
	
	
	if (GetPlayerCount()>=2)
	{
		FString StartGameMessage;
		if (bIsPlaying == false)
		{
			StartGameMessage = FString::Printf(
				TEXT("플레이어 수가 2명 이상이 되어 게임을 3초후 시작합니다.")
				);
		}else
		{
			//진행중이었다면?? 2명이상이었으니 메세지를 다르게
			StartGameMessage = FString::Printf(
			TEXT("새로운 플레이어가 접속하여 게임을 다시 시작합니다.")
			);
		}
		SendSystemNotificationMessage(FString::Printf(TEXT("시작 대기중")));
		SendSystemChattingMessage(StartGameMessage);
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ABNCGameModeBase::ResetGame, 3, false);
		StartAnswerTimer(0);
		
	}
}

void ABNCGameModeBase::ProcessChattingMessage(ABNCPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	ABNCPlayerState* BNCPS = InChattingPlayerController->GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS) == false)
		return;
	
	FString PlayerName;
	PlayerName = BNCPS->GetPlayerNickname();
	if (PlayerName.IsEmpty() == true)
	{
		return;
	}
	
	const bool bIsCurrentTurnPlayer = PlayerControllers.IsValidIndex(CurrentPlayerIndex) && PlayerControllers[CurrentPlayerIndex] == InChattingPlayerController;
	if (bIsPlaying == true && bIsCurrentTurnPlayer && IsGuessingNumberString(InChatMessageString))
	{
		if (BNCPS->GetCurrentGuessCount() >= BNCPS->GetMaxGuessCount())
		{
			//거부 메세지는 해당 플레이어에게만
			FString DenialText = FString::Printf(TEXT("3번까지만 시도 가능합니다"));
			InChattingPlayerController->ClientSetChatMessageString(DenialText);
			return;
		}
		//정답제출
		IncreaseGuessCount(InChattingPlayerController);
		FString ResultString = JudgeResult(InChatMessageString);
		int32 StrikeCount = FCString::Atoi(*ResultString.Left(1));
		
		//
		AdvanceCurrentPlayer();
		
		StartAnswerTimer();
		NotifyCurrentTurnPlayer();
		
		ForEachPlayerController([&](ABNCPlayerController* BNCPlayerController)
		{
			FString JudgeMessageCombined = FString::Printf(
				TEXT("%s : %s -> %s, 남은 횟수 : %d / %d"),
				*BNCPS->GetPlayerNickname(),
				*InChatMessageString,
				*ResultString, 
				BNCPS->GetCurrentGuessCount(),
				BNCPS->GetMaxGuessCount());
			
			//BNCPlayerController->ClientSetChatMessageString(JudgeMessageCombined);
			BNCPlayerController->ClientAddSystemMessage(JudgeMessageCombined);
		});
		if (StrikeCount == 3)
		{
			JudgeGame(InChattingPlayerController, StrikeCount);
		}
		if (AreAllPlayersOutOfGuesses())
		{
			HandleDrawGame();
			//ResetGame();
		}
	}
	else
	{
		//일반 메세지
		ForEachPlayerController([&](ABNCPlayerController* BNCPlayerController)
		{
			FString CommonChatMessageCombined = FString::Printf(
				TEXT("%s : %s"),
				*BNCPS->GetPlayerNickname(),
				*InChatMessageString);
			BNCPlayerController->ClientSetChatMessageString(CommonChatMessageCombined);
		});
	}
}

void ABNCGameModeBase::SendCommonChattingMessage(const FString& InChatMessageString)
{
	ForEachPlayerController([&](ABNCPlayerController* BNCPlayerController)
	{
		BNCPlayerController->ClientSetChatMessageString(InChatMessageString);
	});
}

void ABNCGameModeBase::SendSystemChattingMessage(const FString& SystemMessageString, FColor InColor)
{
	ForEachPlayerController([&](ABNCPlayerController* BNCPlayerController)
	{
		BNCPlayerController->ClientAddSystemMessage(SystemMessageString, InColor);
	});
}

void ABNCGameModeBase::SendSystemNotificationMessage(const FString& SystemMessageString, FColor InColor)
{
	ForEachPlayerController([&](ABNCPlayerController* BNCPlayerController)
	{
		BNCPlayerController->ClientSetNotificationMessage(SystemMessageString, InColor);
	});
}

bool ABNCGameModeBase::IsGuessingNumberString(const FString& InNumber)
{
	// 1. 세자리인가
	if (InNumber.IsEmpty() || InNumber.Len() != 3)
		return false;
	TSet<TCHAR> Chars;
	for (TCHAR C : InNumber)
	{
		// 2. 숫자인가, 3. 중복이 없는가
		if (FChar::IsDigit(C) && C != '0' && !Chars.Contains(C))
		{
			Chars.Add(C);
		}
		else
		{
			return false;
		}
	}
	// 3개 조건을 만족한 경우 true
	return true;
	
}

void ABNCGameModeBase::IncreaseGuessCount(ABNCPlayerController* InChattingPlayerController)
{
	ABNCPlayerState* BNCPS = InChattingPlayerController->GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS))
	{
		BNCPS->SetCurrentGuessCount(BNCPS->GetCurrentGuessCount()+1);
	}
}

void ABNCGameModeBase::ResetGuessCount()
{
	ForEachPlayerController([](ABNCPlayerController* BNCPlayerController)
	{
		ABNCPlayerState* BNCPS = BNCPlayerController->GetPlayerState<ABNCPlayerState>();
		if (IsValid(BNCPS))
		{
			BNCPS->SetCurrentGuessCount(0);
		}
	});
}

void ABNCGameModeBase::JudgeGame(ABNCPlayerController* InPlayerController, int32 StrikeCount)
{
	if (StrikeCount == 3)
	{
		ABNCPlayerState* BNCPS = InPlayerController->GetPlayerState<ABNCPlayerState>();
		FString WinnerString = 
			FString::Printf(TEXT("%s 승리"), *BNCPS->GetPlayerNickname());
		FString ResultMessageString = 
			FString::Printf(TEXT("%s 플레이어가 정답을 맞췄습니다! 5초 후 새 게임을 시작합니다."), *BNCPS->GetPlayerNickname());
		bIsPlaying = false;
		
		SendSystemNotificationMessage(WinnerString, FColor::Green);
		SendSystemChattingMessage(ResultMessageString);
		GetWorldTimerManager().ClearTimer(AnswerTimerHandle);
		StartAnswerTimer(0);
		GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &ABNCGameModeBase::ResetGame, 5, false);
	}
	else
	{
		
	}
}

FString ABNCGameModeBase::JudgeResult(const FString& InGuessNumberString)
{
	int32 Strikes = 0, Balls = 0;
	for (int32 i=0 ; i<3 ; i++)
	{
		if (Answer[i] == InGuessNumberString[i])
			Strikes++;
		else 
		{
			FString GuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (Answer.Contains(GuessChar))
			{
				Balls++;
			}
		}
	}
	if (Strikes == 0 && Balls == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
}

void ABNCGameModeBase::ResetGame()
{
	ForEachPlayerController([](ABNCPlayerController* PlayerController)
	{
		PlayerController->ClearSystemMessage();
	});
	CurrentPlayerIndex = 0;
	//랜덤으로 섞기 알고리즘
	Algo::RandomShuffle(PlayerControllers);
	
	//상태 초기화 하고
	ResetGuessCount();
	GenerateNewAnswer();
	
	if (PlayerControllers.IsEmpty())
	{
		bIsPlaying = false;
		return;
	}
	
	
	bIsPlaying = true;
	NotifyCurrentTurnPlayer();
	StartAnswerTimer();
}

void ABNCGameModeBase::PlayerAnswerTimeout()
{
	//현재 플레이어
	if (PlayerControllers.IsValidIndex(CurrentPlayerIndex) == false)
		return;
	
	ABNCPlayerController* PlayerController = PlayerControllers[CurrentPlayerIndex];
	if (IsValid(PlayerController) == false)
		return;
	ABNCPlayerState* BNCPS = PlayerController->GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS) == false)
		return;
	FString TimeoutPlayername = BNCPS->GetPlayerNickname();
	FString TimeoutMessageString = FString::Printf(TEXT("%s플레이어 타임아웃으로 기회 차감."), *TimeoutPlayername);
	SendSystemChattingMessage(TimeoutMessageString, FColor::Red);
	BNCPS->SetCurrentGuessCount(BNCPS->GetCurrentGuessCount()+1);
	
	if (AreAllPlayersOutOfGuesses())
	{
		HandleDrawGame();
		return;
	}
	
	
	AdvanceCurrentPlayer();
	StartAnswerTimer();
	NotifyCurrentTurnPlayer();
}


void ABNCGameModeBase::GenerateNewAnswer()
{
	Answer.Empty();
	FString NewAnswer;
	TArray<int32> Numbers;
	// 중복없는 3자리 숫자 생성
	do
	{
		int32 Number = FMath::RandRange(1, 9);
		if (Numbers.Contains(Number))
			continue;
		Numbers.Add(Number);
		Answer.Append(FString::FromInt(Number));
	}while (Numbers.Num() < 3);
	UE_LOG(LogTemp, Warning, TEXT("New Answer: %s"), *Answer);
	
}

int32 ABNCGameModeBase::GetPlayerCount()
{
	return PlayerControllers.Num();
}
