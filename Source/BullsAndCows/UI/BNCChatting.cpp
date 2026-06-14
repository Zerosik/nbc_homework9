// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BNCChatting.h"

#include "Components/EditableTextBox.h"
#include "Player/BNCPlayerController.h"

void UBNCChatting::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ChatInputTextBox->OnTextCommitted.IsAlreadyBound(this, &UBNCChatting::OnChattingMessageCommitted) == false)
	{
		ChatInputTextBox->OnTextCommitted.AddDynamic(this, &UBNCChatting::OnChattingMessageCommitted);
	}
}

void UBNCChatting::NativeDestruct()
{
	if (ChatInputTextBox->OnTextCommitted.IsAlreadyBound(this, &UBNCChatting::OnChattingMessageCommitted) == true)
	{
		ChatInputTextBox->OnTextCommitted.RemoveDynamic(this, &UBNCChatting::OnChattingMessageCommitted);
	}
	Super::NativeDestruct();
}

void UBNCChatting::OnChattingMessageCommitted(const FText& CommittedText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ABNCPlayerController* BNCPlayerController = Cast<ABNCPlayerController>(OwningPlayerController);
			if (IsValid(BNCPlayerController) == true)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, 
				// 	CommittedText.ToString());
				
				// 컨트롤러에 메세지 세팅
				BNCPlayerController->SetChatMessageString(CommittedText.ToString());

				// 엔터 쳤으니 텍스트 제거
				ChatInputTextBox->SetText(FText());
			}
		}
		//편리를 위해 입력 포커스 유지
		ChatInputTextBox->SetFocus();
		
	}
	
}
