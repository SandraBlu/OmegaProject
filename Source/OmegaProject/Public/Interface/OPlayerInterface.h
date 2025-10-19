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

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	int32 FindLevelForXP(int32 InXP) const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	int32 GetAttributePtsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	int32 GetAbilityPtsReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player")
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void AddToAttributePts(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	int32 GetAbilityPoints() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void AddToAbilityPts(int32 InAbilityPoints);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void LevelUp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	AOWeapon* GetCurrentWeapon();
};
