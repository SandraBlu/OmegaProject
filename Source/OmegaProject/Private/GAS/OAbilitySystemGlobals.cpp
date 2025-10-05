// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/OAbilitySystemGlobals.h"

#include "OAbilityTypes.h"

FGameplayEffectContext* UOAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FOGameplayEffectContext();
}