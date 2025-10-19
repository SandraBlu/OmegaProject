// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CommonUI_BFL.h"

#include "Kismet/GameplayStatics.h"
#include "UI/UISubsystem.h"

void UCommonUI_BFL::PushHUDNotification(const UObject* WorldContextObject, FText Message, const float Duration)
{
	if (!Message.IsEmptyOrWhitespace())
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContextObject))
		{
			if (UUISubsystem* NSS = GI->GetSubsystem<UUISubsystem>())
			{
				NSS->ShowNotification(Message, Duration);
			}
		}
	}
}
