// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerNotificationWidget.generated.h"

class UScrollBox;
class UTextBlock;
UCLASS()
class BULLSANDCOWS_API UServerNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NotificationTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> MessageScrollBox;

	void SetCurrentNotificationText(const FString& InText, FColor InColor = FColor::White);
	
	void ClearMessages();
};
