// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OUserWidget.h"

#include "UI/UISubsystem.h"

void UOUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UOUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Store the HUD in a subsystem
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UUISubsystem* NSS = GI->GetSubsystem<UUISubsystem>())
			{
				NSS->PlayerHUD = this;
			}
		}
	}
}