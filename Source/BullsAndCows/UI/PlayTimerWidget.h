// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UPlayTimerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* InputTimeoutTextblock;
	
	void UpdateTimerText();
	
private:
	float Timeout = 0;
	
};
