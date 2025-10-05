// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/OCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ODebugHelper.h"
#include "OGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GAS/OAbilitySystemLibrary.h"

void UOCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (HitActor)
	{
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT(" is hitting ") + HitActor->GetActorNameOrLabel());
	}
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);
	
	bool bIsValidBlock = false;
	const FOGameplayTags GameplayTags = FOGameplayTags::Get();
	
	const bool bIsPlayerBlocking = UOAbilitySystemLibrary::NativeDoesActorHaveTag(HitActor, GameplayTags.status_blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UOAbilitySystemLibrary::IsValidBlock(GetOwningPawn(),HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, GameplayTags.status_blocking,EventData);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(),GameplayTags.Event_HitMelee,EventData);
	}
}

