// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterHealth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdated, float, CurrentHealth, float, MaxHealth);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API ICharacterHealth
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FORCEINLINE virtual FOnHealthUpdated& GetOnHealthUpdatedDelegate() { return OnHealthUpdated; }

private:
	FOnHealthUpdated OnHealthUpdated;
};
