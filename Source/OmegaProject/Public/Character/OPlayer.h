// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OCharacterBase.h"
#include "Interface/OPlayerInterface.h"
#include "OPlayer.generated.h"

class UOEquipmentComponent;
class UNiagaraComponent;
class UInputAction;
class UOFootstepComponent;
class UOInputConfig;
class UOAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API AOPlayer : public AOCharacterBase, public IOPlayerInterface
{
	GENERATED_BODY()

public:
	
	AOPlayer();

		virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UOEquipmentComponent* Gear;
	
	//Interfaces

	virtual int32 GetPlayerLevel_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag) override;
	virtual AActor* GetCurrentEquippedWeapon_Implementation(AOWeapon* InWeapon) override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePtsReward_Implementation(int32 Level) const override;
	virtual int32 GetAbilityPtsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePts_Implementation(int32 InAttributePoints) override;
	virtual void AddToAbilityPts_Implementation(int32 InAbilityPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetAbilityPoints_Implementation() const override;
	virtual AOWeapon* GetCurrentWeapon_Implementation() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* LevelUpFX;
	
	UPROPERTY()
	AOWeapon* MeleeWeapon;
	
	UOFootstepComponent* GetFootstepsComp() const;
	
	UFUNCTION(Blueprintpure)
	bool GetIsUsingBlock();
	
	UFUNCTION(BlueprintCallable)
	bool SetIsUsingBlock(bool block );
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SwitchTarget;
	
	UPROPERTY(BlueprintReadOnly)
	UOFootstepComponent* FootstepComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	bool bUsingBlock;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UOInputConfig* InputConfig;

	FVector2D SwitchTargetLock = FVector2D::ZeroVector;
	
	virtual void InitAbilityActorInfo() override;

	//Input Actions
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ClimbAction;*/
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	UPROPERTY()
	UOAbilitySystemComponent* OAbilitySystemComponent;
	
	UOAbilitySystemComponent* GetASC();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpVFX();

	
public:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
