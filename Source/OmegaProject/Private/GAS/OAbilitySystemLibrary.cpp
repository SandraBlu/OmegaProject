// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/OAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "OGameplayTags.h"
#include "Framework/OGameModeBase.h"
#include "Framework/OPlayerState.h"
#include "GAS/DataAsset/OCharacterClassInfo.h"
#include "Interface/OCombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GAS/OHUD.h"
#include "OAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Framework/Countdown/CountdownAction.h"
#include "Framework/Countdown/OEnums.h"
#include "GAS/OAbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/GAS/Controller/OWidgetController.h"


bool UOAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AOHUD*& OutOHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    	{
		OutOHUD = Cast<AOHUD>(PC->GetHUD());
    		if (OutOHUD)
    		{
    			AOPlayerState* PS = PC->GetPlayerState<AOPlayerState>();
    			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
    			UAttributeSet* AS = PS->GetAttributeSet();
    			OutWCParams.AttributeSet = AS;
    			OutWCParams.AbilitySystemComponent = ASC;
    			OutWCParams.PlayerState = PS;
    			OutWCParams.PlayerController= PC;
    			FWidgetControllerParams(PC, PS, ASC, AS);
    			return true;
    		}
    	}
    return false;
}

UOverlayWidgetController* UOAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{

	FWidgetControllerParams WCParams;
	AOHUD* OHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, OHUD))
	{
		return OHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuController* UOAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AOHUD* OHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, OHUD))
	{
		return OHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

UAbilityMenuController* UOAbilitySystemLibrary::GetAbilityMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AOHUD* OHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, OHUD))
	{
			return OHUD->GetAbilityMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UOAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                         ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UOCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharClassInfo ClassDefaultInfo = CharacterClassInfo->GetClassInfo(CharacterClass);

	//Add Source Objects(ASC) and Effect Context and Outgoing Spec for AI Attributes Data Asset: CharacterClassInfo
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttSpecHandle.Data.Get());

	FGameplayEffectContextHandle ResistanceAttributeContextHandle = ASC->MakeEffectContext();
	ResistanceAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle ResistanceAttSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->ResistanceAttributes, Level, ResistanceAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ResistanceAttSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttSpecHandle.Data.Get());
}

void UOAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UOCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharClassInfo& DefaultInfo = CharacterClassInfo->GetClassInfo(CharacterClass);
	for (auto AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UOCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, IOCombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UOCharacterClassInfo* UOAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AOGameModeBase* RGameMode = Cast<AOGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (RGameMode == nullptr) return nullptr;
	return RGameMode->CharacterClassInfo;
}

UOAbilityInfo* UOAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AOGameModeBase* OGameMode = Cast<AOGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (OGameMode == nullptr) return nullptr;
	return OGameMode->AbilityInfo;
}

int32 UOAbilitySystemLibrary::GetXPRewardForEnemySlay(const UObject* WorldContextObject, ECharacterClass CharacterClass,
                                                      int32 CharacterLevel)
{
	UOCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;
	const FCharClassInfo& Info = CharacterClassInfo->GetClassInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

FGameplayEffectContextHandle UOAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FOGameplayTags& GameplayTags = FOGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UOAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
    	for (int32 i = 0; i < NumRotators; i++)
    	{
    		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
    		Rotators.Add(Direction.Rotation());
    	}	
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UOAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}	
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

void UOAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UOAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UOAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UOAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

bool UOAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsBlockedHit();
	}
	return false;
}

bool UOAbilitySystemLibrary::IsDodgedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsDodgedHit();
	}
	return false;
}

bool UOAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsCriticalHit();
	}
	return false;
}

bool UOAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UOAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UOAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UOAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UOAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (REffectContext->GetDamageType().IsValid())
		{
			return *REffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UOAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UOAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UOAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->IsRadialDamage();
	}
	return false;
}

float UOAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UOAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UOAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FOGameplayEffectContext* REffectContext = static_cast<const FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return REffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UOAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UOAbilitySystemLibrary::SetIsDodgedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDodgedHit)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsDodgedHit(bInIsDodgedHit);
	}
}

void UOAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UOAbilitySystemLibrary::SetIsSucessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UOAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UOAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UOAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UOAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		REffectContext->SetDamageType(DamageType);
	}
}

void UOAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,	const FVector& InImpulse)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetDeathImpulse(InImpulse);
	}
}

void UOAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetKnockbackForce(InForce);
	}
}

void UOAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UOAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UOAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UOAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FOGameplayEffectContext* REffectContext = static_cast<FOGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		REffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UOAbilitySystemLibrary::GetTargetsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                                    const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UOCombatInterface>() && !IOCombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(IOCombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UOAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bPlayerTeam = FirstActor->ActorHasTag(FName("player")) && SecondActor->ActorHasTag(FName("player"));
	const bool bEnemyTeam = FirstActor->ActorHasTag(FName("enemy")) && SecondActor->ActorHasTag(FName("enemy"));
	const bool bFriend = bPlayerTeam || bEnemyTeam;
	return !bFriend;
}

UOAbilitySystemComponent* UOAbilitySystemLibrary::NativeGetRASCFromActor(AActor* InActor)
{
	check(InActor);
	return CastChecked<UOAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UOAbilitySystemLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UOAbilitySystemComponent* ASC = NativeGetRASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UOAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UOAbilitySystemComponent* ASC = NativeGetRASCFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UOAbilitySystemLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UOAbilitySystemComponent* ASC = NativeGetRASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

 void UOAbilitySystemLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EConfirmType& OutConfirmType)
{
 	OutConfirmType = NativeDoesActorHaveTag(InActor,TagToCheck)? EConfirmType::Yes : EConfirmType::No;
}

UOCombatComponent* UOAbilitySystemLibrary::NativeGetCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IOCombatInterface* CombatInterface = Cast<IOCombatInterface>(InActor))
	{
		return CombatInterface->GetCombatComponent();
	}
	return nullptr;
}

UOCombatComponent* UOAbilitySystemLibrary::BP_GetCombatComponentFromActor(AActor* InActor, EValidType& OutValidType)
{
	UOCombatComponent* CombatComponent = NativeGetCombatComponentFromActor(InActor);
	OutValidType = CombatComponent? EValidType::Valid : EValidType::Invalid;
	return CombatComponent;
}

float UOAbilitySystemLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& ScalableFloat, float InLevel)
{
	return ScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UOAbilitySystemLibrary::ComputeHitReactDirection(AActor* InAttacker, AActor* InTarget, float& OutHitDirection)
{
	check(InAttacker && InTarget);
	const FVector TargetForward = InTarget->GetActorForwardVector();
	const FVector TargetAngleNormalized = (InAttacker->GetActorLocation() - InTarget->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(TargetForward,TargetAngleNormalized);
	OutHitDirection = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(TargetForward, TargetAngleNormalized);
	if (CrossResult.Z < 0.f)
	{
		OutHitDirection *= -1.f;
	}

	if (OutHitDirection >= -45.f && OutHitDirection <= 45.f)
	{
		return FGameplayTag::RequestGameplayTag(FName("event.HitReact.front"));
	}
	else if (OutHitDirection<-45.f && OutHitDirection>=-135.f)
	{
		return FGameplayTag::RequestGameplayTag(FName("event.HitReact.left"));
	}
	else if (OutHitDirection<-135.f || OutHitDirection>135.f)
	{
		return FGameplayTag::RequestGameplayTag(FName("event.HitReact.back"));
	}
	else if(OutHitDirection>45.f && OutHitDirection<=135.f)
	{
		return FGameplayTag::RequestGameplayTag(FName("event.HitReact.right"));
	}
	
	return FGameplayTag::RequestGameplayTag(FName("event.HitReact.front"));
}

bool UOAbilitySystemLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	const FString DebugString = FString::Printf(TEXT("DotResult: %f %s"), DotResult, DotResult<0.f? TEXT("Valid Block") : TEXT("NO Block"));
	//Debug::Print(DebugString,DotResult<0.f? FColor::Green : FColor::Red);
	return DotResult < 0.f? true : false;;
}

void UOAbilitySystemLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,float& OutRemainingTime, ECountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") ECountDownActionOutput& CountDownOutput,FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;
	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	if (!World)
	{
		return;
	}
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	FCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);
	if (CountDownInput == ECountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FCountDownAction(TotalTime, UpdateInterval, OutRemainingTime, CountDownOutput, LatentInfo));
		}
	}
	if (CountDownInput == ECountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}
