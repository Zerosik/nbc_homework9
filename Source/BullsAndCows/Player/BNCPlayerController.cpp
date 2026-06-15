// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BNCPlayerController.h"

#include "BNCPlayerState.h"
#include "Components/ScrollBox.h"
#include "Game/BNCGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BNCChatting.h"
#include "UI/BNCChatMessage.h"
#include "UI/PlayTimerWidget.h"
#include "UI/ServerNotificationWidget.h"

ABNCPlayerController::ABNCPlayerController()
{
	
}

void ABNCPlayerController::CreateChatInputWidget()
{
	if (IsValid(ChatInputWidgetClass) == false)
		return;
	
	ChatInputWidgetInstance = CreateWidget<UBNCChatting>(this, ChatInputWidgetClass);
	if (IsValid(ChatInputWidgetInstance))
	{
		ChatInputWidgetInstance->AddToViewport();
		FString InfoString = FString::Printf(TEXT("먼저 닉네임을 입력해주세요"));
		AddChatMessage(InfoString, FColor::Orange);
	}
}

void ABNCPlayerController::CreateNotificationWidget()
{
	if (IsValid(NotificationWidgetClass) == false)
		return;
	
	NotificationWidgetInstance = CreateWidget<UServerNotificationWidget>(this, NotificationWidgetClass);
	if (IsValid(NotificationWidgetInstance))
	{
		NotificationWidgetInstance->AddToViewport();
	}
}

void ABNCPlayerController::CreateTimeoutWidget()
{
	if (IsValid(TimeoutWidgetClass) == false)
		return;
	
	TimeoutWidgetInstance = CreateWidget<UPlayTimerWidget>(this, TimeoutWidgetClass);
	if (IsValid(TimeoutWidgetInstance))
	{
		TimeoutWidgetInstance->AddToViewport();
	}
}

void ABNCPlayerController::AddMessageToScrollBox(UScrollBox* TargetScrollBox, const FString& InChatMessageString, FColor InColor)
{
	if (IsValid(ChatMessageWidgetClass) == false || IsValid(TargetScrollBox) == false)
		return;
	
	UBNCChatMessage* ChatMessage = CreateWidget<UBNCChatMessage>(this, ChatMessageWidgetClass);
	if (IsValid(ChatMessage))
	{
		ChatMessage->SetChatMessage(InChatMessageString, InColor);
		TargetScrollBox->AddChild(ChatMessage);
		TargetScrollBox->ScrollToEnd();
		TargetScrollBox->bAnimateWheelScrolling = true;
	}
}

void ABNCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() == false)
		return;
	SetShowMouseCursor(true);
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	
	CreateChatInputWidget();
	CreateNotificationWidget();
	CreateTimeoutWidget();
}

void ABNCPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	if (IsLocalController() == false)
		return;
	
	//if (InChatMessageString == "Exit")
	//	ServerLogout();
	/*
	ABNCPlayerState* BNCPS = GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS) && BNCPS->GetPlayerNickname().IsEmpty() == true)
	{
		// 닉네임이 replicated지만, 클라이언트에서는 적용되지 않으니, 서버에서 하도록 변경해야함.
		BNCPS->SetPlayerNickname(InChatMessageString);
		FString InfoString = FString::Printf(TEXT("당신의 닉네임은 %s 입니다."), *InChatMessageString);
		AddChatMessage(InfoString, FColor::Orange);
		return;
	}
	FString NameString = FString::Printf(TEXT("PlayerName : %s"),  *BNCPS->GetPlayerNickname());
	*/
	
	FString InChatMessageStringTrimmed = InChatMessageString.TrimStartAndEnd();
	
	// 서버로 메시지 전달
	ServerSetChatMessageString(InChatMessageStringTrimmed);
	
	
}

void ABNCPlayerController::AddChatMessage(const FString& InChatMessageString, FColor InColor)
{
	if (IsLocalController() == false)
		return;
	
	AddMessageToScrollBox(IsValid(ChatInputWidgetInstance) ? ChatInputWidgetInstance->ChatScrollBox : nullptr, InChatMessageString, InColor);
}

void ABNCPlayerController::ServerLogout_Implementation()
{
	if (HasAuthority())
	{
		AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
		if (IsValid(GM))
			GM->Logout(this);
		
	}
}

void ABNCPlayerController::ClearSystemMessage_Implementation()
{
	if (IsValid(NotificationWidgetInstance))
	{
		NotificationWidgetInstance->ClearMessages();
	}
}

void ABNCPlayerController::ClientSetNotificationMessage_Implementation(const FString& InChatMessageString,
                                                                       FColor InColor)
{
	if (IsValid(NotificationWidgetInstance))
	{
		NotificationWidgetInstance->SetCurrentNotificationText(InChatMessageString, InColor);
	}
}

void ABNCPlayerController::ClientAddSystemMessage_Implementation(const FString& InChatMessageString, FColor InColor)
{
	if (IsLocalController() == false)
		return;
	
	AddMessageToScrollBox(IsValid(NotificationWidgetInstance) ? NotificationWidgetInstance->MessageScrollBox : nullptr, InChatMessageString, InColor);
}

void ABNCPlayerController::MulticastSendMessageString_Implementation(const FString& InChatMessageString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ABNCPlayerController* CXPC = Cast<ABNCPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				// {PlayerName} 접속 메세지 출력
				//FString NotificationString = InNameString + TEXT(" has joined the game.");
				CXPC->ClientSetChatMessageString(FString::Printf(TEXT("MulticastSendMessageString")));
			}
		}
	}
}

void ABNCPlayerController::ServerSetChatMessageString_Implementation(const FString& InChatMessageString)
{
	// 게임모드는 서버에만 있으므로 HasAuthority 체크 안해도됨
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	
	ABNCPlayerState* BNCPS = GetPlayerState<ABNCPlayerState>();
	if (IsValid(BNCPS) && BNCPS->GetPlayerNickname().IsEmpty() == true)
	{
		// 닉네임이 replicated지만, 클라이언트에서는 적용되지 않으니, 서버에서 하도록 변경해야함.
		BNCPS->SetPlayerNickname(InChatMessageString);
		//FString InfoString = FString::Printf(TEXT("플레이어 %s 가 접속하였습니다."), *InChatMessageString);
		//AddChatMessage(InfoString, FColor::Orange);
		ABNCGameModeBase* BNCGM = Cast<ABNCGameModeBase>(GM);
		if (IsValid(BNCGM))
		{
			BNCGM->PlayerCustomLogin(this);
			//일반 전체메시지
			//BNCGM->SendCommonChattingMessage(InfoString);
		}
		return;
	}
	
	
	if (IsValid(GM))
	{
		ABNCGameModeBase* BNCGM = Cast<ABNCGameModeBase>(GM);
		if (IsValid(BNCGM))
		{
			BNCGM->ProcessChattingMessage(this, InChatMessageString);
			// 게임모드에 메세지 보내고, 게임모드에서는 메세지에 따른 처리(그냥채팅 or 정답체크)
			
		}
	}
}

void ABNCPlayerController::ClientSetChatMessageString_Implementation(const FString& InChatMessageString, FColor InColor)
{
	//서버에서 보낸 메세지 처리
	AddChatMessage(InChatMessageString, InColor);
}
