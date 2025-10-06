// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickSignature,float,DeltaTime);

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UAbilityTask_OnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_OnTick();

	UFUNCTION(BlueprintCallable, Category = "AbilityTasks", meta = (HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static UAbilityTask_OnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	//~ Begin UGameplayTask Interface
	virtual void TickTask(float DeltaTime) override;
	//~ End UGameplayTask Interface

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickSignature OnAbilityTaskTickDelegate;
	
};
