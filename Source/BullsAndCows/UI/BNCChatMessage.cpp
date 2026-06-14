// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BNCChatMessage.h"

#include "Components/TextBlock.h"

void UBNCChatMessage::SetChatMessage(const FString& InChatMessage, FColor InColor = FColor::White)
{
	if (IsValid(MessageTextBlock))
	{
		MessageTextBlock->SetText(FText::FromString(InChatMessage));
		MessageTextBlock->Font.Size = 20;
		MessageTextBlock->ColorAndOpacity = InColor;
	}
}
