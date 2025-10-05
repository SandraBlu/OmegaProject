// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "OPlayerController.generated.h"

class UDamageTextWidget;
/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API AOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	AOPlayerController();

	virtual FGenericTeamId GetGenericTeamId() const;
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bDodgedHit, bool bCriticalHit);
	
private:

	FGenericTeamId PlayerTeamID;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidget> DamageTextComponentClass;
	
};
