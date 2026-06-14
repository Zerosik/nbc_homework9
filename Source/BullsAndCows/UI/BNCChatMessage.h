// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BNCChatMessage.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UBNCChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MessageTextBlock;
	
	void SetChatMessage(const FString& InChatMessage, FColor InColor);
	
};
