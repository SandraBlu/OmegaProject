// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GAS/DataAsset/OCharacterClassInfo.h"
#include "UObject/Interface.h"
#include "OCombatInterface.generated.h"

class UOCombatComponent;
class AOWeapon;
class UNiagaraSystem;
class UAbilitySystemComponent;
class OCharacterBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OMEGAPROJECT_API IOCombatInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	void GetHit(const FVector& ImpactPoint);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Combat")
	int32 GetPlayerLevel();

	virtual UOCombatComponent* GetCombatComponent() const = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	FVector GetCombatSocketLocation(const FGameplayTag& CombatSocketTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UAnimMontage* GetStunnedMontage();
	
	virtual void Die(const FVector& DeathImpulse) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	bool IsDead() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
	void FaceTarget(const FVector& Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	void SetCombatTarget(AActor* InCombatTarget);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	ECharacterClass GetCharacterClass();
	
	virtual FOnASCRegistered GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeath GetOnDeathDelegate() = 0;
	virtual FOnDamageSignature& GetOnDamageSignature() = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	AActor* GetCurrentEquippedWeapon(AOWeapon* InWeapon);

	//Targeting
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnTargeted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
	void OnTargetedEnd();
};
