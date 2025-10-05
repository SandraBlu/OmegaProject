// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OGameModeBase.generated.h"

enum class EGameDifficulty : uint8;
class UOAbilityInfo;
class UOCharacterClassInfo;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API AOGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	UOCharacterClassInfo* CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	UOAbilityInfo* AbilityInfo;

	FORCEINLINE EGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty;}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
	EGameDifficulty CurrentGameDifficulty;
	
};
