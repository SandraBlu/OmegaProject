// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OGameplayAbility.generated.h"

class UOAbilitySystemComponent;
class AOPlayer;
/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:

	float GetEnergyCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UOAbilitySystemComponent* GetRAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AOPlayer* GetPlayerFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	virtual class ANPCBase* GetNPCCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	virtual class AOPlayerController* GetPlayerControllerFromActorInfo();

private:
	
	TWeakObjectPtr<AOPlayer> CachedPlayer;
	TWeakObjectPtr<ANPCBase> CachedNPC;
	TWeakObjectPtr<AOPlayerController> CachedPlayerController;
	
};
