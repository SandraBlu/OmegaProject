// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OPlayerState.generated.h"

class UOLevelUpInfo;
class UOInventoryComponent;
class UAttributeSet;
class UAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32)
/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API AOPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AOPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UOInventoryComponent* PlayerInventory;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UOLevelUpInfo* LevelUpInfo;
	
	FOnPlayerStatChanged OnXPChangeDelegate;
	FOnPlayerStatChanged OnLevelChangeDelegate;
	FOnPlayerStatChanged OnAttributePtsChangeDelegate;
	FOnPlayerStatChanged OnAbilityPtsChangeDelegate;

	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePts() const { return AttributePoints; }
	FORCEINLINE int32 GetAbilityPts() const { return AbilityPoints; }

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePts(int32 InPoints);
	void AddToAbilityPts(int32 InPoints);
	
	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);

protected:
	
	UPROPERTY(VisibleAnywhere)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AbilityPoints)
	int32 AbilityPoints = 0;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_AbilityPoints(int32 OldAbilityPoints);
};
