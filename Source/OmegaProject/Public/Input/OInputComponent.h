// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "OInputConfig.h"
#include "OInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	
	template<class UserClass, typename CallBackFunc>
	void BindAbilityActions(const UOInputConfig* InputConfig, UserClass* Object, CallBackFunc PressedFunc, CallBackFunc  ReleasedFunc);
	
};

template<class UserClass, typename CallBackFunc>
	void UOInputComponent::BindAbilityActions(const UOInputConfig* InputConfig, UserClass* Object, CallBackFunc PressedFunc, CallBackFunc ReleasedFunc)
{
	checkf(InputConfig,TEXT("Input config data asset is null,can not proceed with binding"));

	for (const FOInputAction& Action : InputConfig->AbilityInput)
	{
		if (!Action.IsValid()) continue;
		{
			BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
		}
	}
}