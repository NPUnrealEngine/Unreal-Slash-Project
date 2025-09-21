// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponDropped);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FOnWeaponEquipped OnWeaponEquipped;
	FOnWeaponDropped OnWeaponDropped;

public:
	virtual bool IsEquipped() = 0;
};
