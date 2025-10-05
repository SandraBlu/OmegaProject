#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "OAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
