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
		WeaponInterface->OnWeaponEquipped.AddDynamic(this, &UPickupIndicatorComponent::SetVisible);
		WeaponInterface->OnWeaponDropped.AddDynamic(this, &UPickupIndicatorComponent::SetInvisible);

		bHiddenInGame = WeaponInterface->IsEquipped();
	}
}

void UPickupIndicatorComponent::SetVisible()
{
	bHiddenInGame = true;
}

void UPickupIndicatorComponent::SetInvisible()
{
	bHiddenInGame = false;
}
