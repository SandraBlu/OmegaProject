// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OPlayerInterface.generated.h"

class AOWeapon;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OMEGAPROJECT_API IOPlayerInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePtsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPtsReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePts(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToAbilityPts(int32 InAbilityPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AOWeapon* GetCurrentWeapon();
};
