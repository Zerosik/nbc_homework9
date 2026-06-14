// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerNotificationWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UServerNotificationWidget::SetCurrentNotificationText(const FString& InText, FColor InColor)
{
	if (IsValid(NotificationTextBlock))
	{
		NotificationTextBlock->SetText(FText::FromString(InText));
		NotificationTextBlock->Font.Size = 20;
		NotificationTextBlock->ColorAndOpacity = InColor;
	}
}

void UServerNotificationWidget::ClearMessages()
{
	if (IsValid(MessageScrollBox))
	{
		MessageScrollBox->ClearChildren();
	}
}
