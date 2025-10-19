// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UISubsystem.h"

#include "UI/OUserWidget.h"


void UUISubsystem::ShowNotification(const FText& NotificationText, const float Duration)
{
	if (PlayerHUD && !NotificationText.IsEmptyOrWhitespace())
	{
		PlayerHUD->ShowNotification(NotificationText, Duration);
	}
}
