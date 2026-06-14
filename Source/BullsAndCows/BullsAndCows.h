// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define NETMODE_TCHAR ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("Client%02d"), UE::GetPlayInEditorID()) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("StandAlone") : TEXT("Server")))
#define FUNCTION_TCHAR (ANSI_TO_TCHAR(__FUNCTION__))
#define DX_LOG_NET(LogCategory, Verbosity, Format, ...) UE_LOG(LogCategory, Verbosity, TEXT("[%s] %s %s"), NETMODE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))

#define LOCAL_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define REMOTE_ROLE_TCHAR *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define DX_LOG_ROLE(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), NETMODE_TCHAR, LOCAL_ROLE_TCHAR, REMOTE_ROLE_TCHAR, FUNCTION_TCHAR, *FString::Printf(Format, ##__VA_ARGS__))


class BullsAndCowsFunctionLibrary
{
	
public:
	static void PrintChattingString(const AActor* InWorldContext, FString &InMessage, float InDisplayTime)
	{
		if (IsValid(InWorldContext) && IsValid(GEngine))
		{
			if (InWorldContext->GetNetMode() == NM_Client || InWorldContext->GetNetMode() == NM_ListenServer)
			{
				// 클라이언트 출력, 추후 UI에 올리기
				GEngine->AddOnScreenDebugMessage(-1, InDisplayTime, FColor::White, InMessage);
			}else
			{
				// 서버 출력은 UE_LOG
				UE_LOG(LogTemp, Warning, TEXT("%s"), *InMessage);
			}
		}
	}
};