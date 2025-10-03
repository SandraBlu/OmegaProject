// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DataAsset/OAbilityInfo.h"

FRAbilityInfo UOAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FRAbilityInfo& Info : AbilityInfo)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find info for AbilityTag[%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FRAbilityInfo();
}
