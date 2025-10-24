// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OUserWidget.h"

#include "UI/UISubsystem.h"

void UOUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Store OUserWidget in a subsystem - that way, 
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

void UOUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
