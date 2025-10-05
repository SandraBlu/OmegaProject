// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "UObject/NoExportTypes.h"
#include "OWidgetController.generated.h"

class UAbilitySystemComponent;
class UOAttributeSet;
class UOAbilitySystemComponent;
class AOPlayerState;
class AOPlayerController;
class UOAbilityInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) 
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerController* PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APlayerState* PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAttributeSet* AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class OMEGAPROJECT_API UOWidgetController : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Ability")
	FAbilityInfoSignature AbilityInfoDelegate;

	void BroadcastAbilityInfo();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Data")
	TObjectPtr<UOAbilityInfo> AbilityInfo;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	APlayerState* PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	AOPlayerController* OPC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	AOPlayerState* OPS;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UOAbilitySystemComponent* OASC;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UOAttributeSet* OAS;

	AOPlayerController* GetOPC();
	AOPlayerState* GetOPS();
	UOAbilitySystemComponent* GetOASC();
	UOAttributeSet* GetOAS();
	
};
