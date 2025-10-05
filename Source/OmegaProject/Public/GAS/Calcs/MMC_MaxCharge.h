// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxCharge.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UMMC_MaxCharge : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UMMC_MaxCharge();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
	
};
