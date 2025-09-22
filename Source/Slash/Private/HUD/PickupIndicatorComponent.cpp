// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PickupIndicatorComponent.h"
#include <HUD/PickupIndicator.h>
#include <Interfaces/WeaponInterface.h>
#include <EnhancedInputSubsystems.h>
#include <Components/TextBlock.h>

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

	/**
	 * Delay for 1 second before setup pick up key display in widget
	 * as Input Mapping Context take a moment to setup.
	 */
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			DelaySetupKeyTimer,
			this,
			&UPickupIndicatorComponent::SetupKeyDisplay,
			1.f,
			false
		);
	}

	bHiddenInGame = true;
}

void UPickupIndicatorComponent::SetupKeyDisplay()
{
	UWorld* World = GetWorld();
	if (World && PickupAction && PickupIndicatorWidget)
	{
		if (ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				TArray<FKey> Keys = EnhancedInputSubsystem->QueryKeysMappedToAction(PickupAction);
				if (!Keys.IsEmpty())
				{
					PickupIndicatorWidget->PickupKeyText->SetText(Keys[0].GetDisplayName());
				}
			}

		}
	}
}

void UPickupIndicatorComponent::OnEquipped()
{
	bEquipped = true;
	UpdateVisibility();
}

void UPickupIndicatorComponent::OnDropped()
{
	bEquipped = false;
	UpdateVisibility();
}

void UPickupIndicatorComponent::OnBeginOverlapPlayer(APawn* Pawn, APlayerController* PlayerController)
{
	bOverlapPlayer = true;
	UpdateVisibility();
}

void UPickupIndicatorComponent::OnEndOverlapPlayer(APawn* Pawn, APlayerController* PlayerController)
{
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
