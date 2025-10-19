// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonUI_BFL.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UCommonUI_BFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//Add a notification to the Narrative HUD, provided one has been created 
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "PlayerHUD", meta = (DisplayName = "Show HUD Notification", WorldContext = "WorldContextObject"))
	static void PushHUDNotification(const UObject* WorldContextObject, FText Message, const float Duration = 5.f);
	
};
