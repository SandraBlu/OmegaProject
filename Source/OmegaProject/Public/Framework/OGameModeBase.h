// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OGameModeBase.generated.h"

class UOCharacterClassInfo;
enum class EOGameDifficulty : uint8;
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

//	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
//	UAbilityInfo* AbilityInfo;

	//FORCEINLINE EOGameDifficulty GetCurrentGameDifficulty() const { return CurrentGameDifficulty;}

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Settings")
//	EOGameDifficulty CurrentGameDifficulty;
	
};
