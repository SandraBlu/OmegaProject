// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DataAsset/OCharacterClassInfo.h"

FCharClassInfo UOCharacterClassInfo::GetClassInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}
