// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OAbilitySystemLibrary.generated.h"

class AMasterHUD;
enum class ECountDownActionInput : uint8;
class UOAbilitySystemComponent;
class UOAbilityInfo;
enum class ECharacterClass : uint8;
class UAbilityMenuController;
class UAbilitySystemComponent;
class UAttributeMenuController;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UOCombatComponent;
class UOCharacterClassInfo;

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

	//Widget Controllers
	UFUNCTION(BlueprintPure, Category = "OBFL|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AMasterHUD*& OutOHUD);

	UFUNCTION(BlueprintPure, Category = "OBFL|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "OBFL|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuController* GetAttributeMenuController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "OBFL|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAbilityMenuController* GetAbilityMenuController(const UObject* WorldContextObject);
	
	//Initializers
	 UFUNCTION(BlueprintCallable, Category = "OBFL|CharacterClassDefaults")
	 static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
	
	 UFUNCTION(BlueprintCallable, Category = "OBFL|CharacterClassDefaults")
	 static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	//Class/AbilityInfo
	 UFUNCTION(BlueprintCallable, Category="OBFL|CharacterClassDefaults")
	 static UOCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="OBFL|CharacterClassDefaults")
	static UOAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	//Getters Damage Parameters
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static bool IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "OBFL|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	//Setters Damage Parameters
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetIsDodgedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodgedHit);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetIsSucessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInDebuffDuration);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInDebuffFrequency);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FGameplayTag& InDamageType);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InImpulse);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InForce);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);
	UFUNCTION(BlueprintCallable, Category = "OBFL|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InOrigin);
	

	//Gameplay Ability Functions
	UFUNCTION(BlueprintCallable, Category = "AOBFL|GAMelee")
	static void GetTargetsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AOBFL|GAMelee")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	static int32 GetXPRewardForEnemySlay(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, Category = "OBFL|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	//Target Ability
	UFUNCTION(BlueprintPure, Category = "OBFL|GAS")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	UFUNCTION(BlueprintPure, Category = "OBFL|GAS")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	//Damage Effect Params
	UFUNCTION(BlueprintCallable, Category = "OBFL|DamageEffect")
	static void SetIsRadialDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);
	
	UFUNCTION(BlueprintCallable, Category = "OBFL|DamageEffect")
	static void SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable, Category = "OBFL|DamageEffect")
	static void SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable, Category = "OBFL|DamageEffect")
	static void SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);

	//--- added Melee CombatFunctions

	static UOAbilitySystemComponent* NativeGetRASCFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "OBFL")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "OBFL")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor,FGameplayTag TagToRemove);
	
	static bool NativeDoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "OBFL", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,EConfirmType& OutConfirmType);
	
	static UOCombatComponent* NativeGetCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "OBFL", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UOCombatComponent* BP_GetCombatComponentFromActor(AActor* InActor,EValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "OBFL", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& ScalableFloat, float InLevel = 1.f);
	
	UFUNCTION(BlueprintPure, Category = "OBFL", meta = (CompactNodeTitle = "Get Value At Level"))
	static FGameplayTag ComputeHitReactDirection(AActor* InAttacker, AActor* InTarget, float& HitDirection);
	
	UFUNCTION(BlueprintPure, Category = "OBFL")
	static bool IsValidBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintCallable, Category = "OBFL", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, ECountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") ECountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);
};
