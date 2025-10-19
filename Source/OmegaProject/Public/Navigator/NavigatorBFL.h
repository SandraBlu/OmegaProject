// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NavigatorBFL.generated.h"

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UNavigatorBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#if WITH_EDITOR

	UFUNCTION(BlueprintPure, Category = "Navigator Function Library")
	static class UTexture2DFactoryNew* GetTexture2DFactory();

#endif 
	
};
