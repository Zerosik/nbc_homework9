// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayTimerWidget.h"
#include "Game/BNCGameState.h"
#include "Components/TextBlock.h"

void UPlayTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateTimerText();
}

void UPlayTimerWidget::UpdateTimerText()
{    
	//UE_LOG(LogTemp, Warning, TEXT("UPlayTimerWidget::UpdateTimerText"));
	ABNCGameState* BNCGS = GetWorld()->GetGameState<ABNCGameState>();
	if (BNCGS->GetTimeoutLeft()>0)
	{
		//SetVisibility(ESlateVisibility::Visible);
		const int32 Minutes = FMath::FloorToInt(BNCGS->GetTimeoutLeft() / 60.f);
		const int32 Seconds = FMath::FloorToInt(FMath::Fmod(BNCGS->GetTimeoutLeft(), 60.0f));
		InputTimeoutTextblock->SetText(FText::FromString(FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds)));
	}else
	{
		//SetVisibility(ESlateVisibility::Collapsed);
		InputTimeoutTextblock->SetText(FText::FromString(FString::Printf(TEXT(""))));
	}
}
