// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "OUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Notifications")
	void ShowNotification(const FText& NotificationText, const float Duration = 5.f);

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	UObject* WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
