// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BNCChatting.generated.h"

class UEditableTextBox;
class UScrollBox;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UBNCChatting : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInputTextBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatScrollBox;
	
	UFUNCTION(BlueprintCallable)
	void OnChattingMessageCommitted(const FText& CommittedText, ETextCommit::Type CommitMethod);
};