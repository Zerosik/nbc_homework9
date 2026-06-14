// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BNCPlayerController.generated.h"

class UBNCChatting;
class UBNCChatMessage;
class UServerNotificationWidget;
class UPlayTimerWidget;
class UScrollBox;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABNCPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABNCPlayerController();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayTimerWidget> TimeoutWidgetClass;

	UPROPERTY()
	TObjectPtr<UPlayTimerWidget> TimeoutWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UServerNotificationWidget> NotificationWidgetClass;

	UPROPERTY()
	TObjectPtr<UServerNotificationWidget> NotificationWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBNCChatting> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UBNCChatting> ChatInputWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBNCChatMessage> ChatMessageWidgetClass;
	
	// 입력받은 메세지 처리
	void SetChatMessageString(const FString &InChatMessageString);

	// 메세지를 채팅UI에 추가하기
	void AddChatMessage(const FString &InChatMessageString, FColor InColor = FColor::White);	
	
	// 클라이언트에서 서버로 채팅메세지 보내기
	UFUNCTION(Server, Reliable)
	void ServerSetChatMessageString(const FString &InChatMessageString);
	
	// 서버에서 클라이언트 채팅메시지 받아서 출력
	UFUNCTION(Client, Reliable)
	void ClientSetChatMessageString(const FString &InChatMessageString, FColor InColor = FColor::White);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSendMessageString(const FString &InChatMessageString);
	
	
	// 서버에서 클라이언트 채팅메시지 받아서 출력
	UFUNCTION(Client, Reliable)
	void ClientAddSystemMessage(const FString &InChatMessageString, FColor InColor = FColor::White);
	UFUNCTION(Client, Reliable)
	void ClientSetNotificationMessage(const FString &InChatMessageString, FColor InColor = FColor::White);
	UFUNCTION(Client, Reliable)
	void ClearSystemMessage();
private:
	void CreateChatInputWidget();
	void CreateNotificationWidget();
	void CreateTimeoutWidget();
	void AddMessageToScrollBox(UScrollBox* TargetScrollBox, const FString& InChatMessageString, FColor InColor);
	
	UFUNCTION(Server, Reliable)
	void ServerLogout();
	
};
