// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PickupIndicatorComponent.h"
#include <HUD/PickupIndicator.h>
#include <Interfaces/WeaponInterface.h>

void UPickupIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	PickupIndicatorWidget = Cast<UPickupIndicator>(GetUserWidgetObject());
	if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(GetOwner()))
	{
		WeaponInterface->OnWeaponEquipped.AddDynamic(this, &UPickupIndicatorComponent::OnEquipped);
		WeaponInterface->OnWeaponDropped.AddDynamic(this, &UPickupIndicatorComponent::OnDropped);
		WeaponInterface->OnWeaponBeginOverlapPlayer.AddDynamic(
			this,
			&UPickupIndicatorComponent::OnBeginOverlapPlayer
		);
		WeaponInterface->OnWeaponEndOverlapPlayer.AddDynamic(
			this,
			&UPickupIndicatorComponent::OnEndOverlapPlayer
		);
	}

	bHiddenInGame = true;
}

void UPickupIndicatorComponent::OnEquipped()
{
	UE_LOG(LogTemp, Warning, TEXT("%s equipped"), *GetOwner()->GetName());
	bEquipped = true;
	UpdateVisibility();
}

void UPickupIndicatorComponent::OnDropped()
{
	UE_LOG(LogTemp, Warning, TEXT("%s dropped"), *GetOwner()->GetName());
	bEquipped = false;
	UpdateVisibility();
}

void UPickupIndicatorComponent::OnBeginOverlapPlayer(APawn* Pawn, APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("%s begin overlap player"), *GetOwner()->GetName());
	bOverlapPlayer = true;
	UpdateVisibility();
}

void UPickupIndicatorComponent::OnEndOverlapPlayer(APawn* Pawn, APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("%s end overlap player"), *GetOwner()->GetName());
	bOverlapPlayer = false;
	UpdateVisibility();
}

void UPickupIndicatorComponent::UpdateVisibility()
{
	if (bEquipped) bHiddenInGame = true;
	else
	{
		if (bOverlapPlayer) bHiddenInGame = false;
		else bHiddenInGame = true;
	}
}
