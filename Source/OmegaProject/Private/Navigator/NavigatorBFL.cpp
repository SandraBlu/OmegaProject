// Fill out your copyright notice in the Description page of Project Settings.


#include "Navigator/NavigatorBFL.h"

#include "Factories/Texture2dFactoryNew.h"

class UTexture2DFactoryNew* UNavigatorBFL::GetTexture2DFactory()
{
	return NewObject<UTexture2DFactoryNew>();
}
