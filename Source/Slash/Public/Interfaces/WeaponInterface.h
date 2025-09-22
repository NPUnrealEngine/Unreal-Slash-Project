// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

/**
 * Delegate weapon equipped.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponEquipped);

/**
 * Delegate weapon dropped.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponDropped);

/**
 * Delegate weapon begin overlap player.
 * 
 * \param Pawn: A pawn control by player
 * \param PlayerController: Player controller
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponBeginOverlapPlayer, APawn*, Pawn, APlayerController*, PlayerController);

/**
 * Delegate weapon end overlap player.
 *
 * \param Pawn: A pawn control by player
 * \param PlayerController: Player controller
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEndOverlapPlayer, APawn*, Pawn, APlayerController*, PlayerController);

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
	FOnWeaponBeginOverlapPlayer OnWeaponBeginOverlapPlayer;
	FOnWeaponEndOverlapPlayer OnWeaponEndOverlapPlayer;

public:
	virtual bool IsEquipped() = 0;
};
