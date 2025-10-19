// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

/**
 * provide efficient access to the HUD,
 */
UCLASS()
class OMEGAPROJECT_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public: 	

	//Cached common hud 
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	class UOUserWidget* PlayerHUD;

	//Send a notification to the player HUD
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowNotification(const FText& NotificationText, const float Duration = 5.f);
	
};
