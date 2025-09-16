// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include <HUD/HealthBar.h>
#include <Components/ProgressBar.h>
#include <Interfaces/HitInterface.h>
#include <Interfaces/CharacterHealth.h>


//void UHealthBarComponent::SetHealthPercent(float Percent)
//{
//	if (HealthBarWidget == nullptr)
//	{
//		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
//	}
//
//	if (HealthBarWidget && HealthBarWidget->HealthBar)
//	{
//		HealthBarWidget->HealthBar->SetPercent(Percent);
//	}
//}

void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	/* Listen to character's health update event */
	if(ICharacterHealth* HealthInterface = Cast<ICharacterHealth>(GetOwner()))
	{
		FOnHealthUpdated& OnHealthUpdateDelegate = HealthInterface->GetOnHealthUpdatedDelegate();
		OnHealthUpdateDelegate.AddDynamic(this, &UHealthBarComponent::UpdateHealth);
	}
}

void UHealthBarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	/* Unbind event */
	if (ICharacterHealth* HealthInterface = Cast<ICharacterHealth>(GetOwner()))
	{
		FOnHealthUpdated& OnHealthUpdateDelegate = HealthInterface->GetOnHealthUpdatedDelegate();
		OnHealthUpdateDelegate.RemoveDynamic(this, &UHealthBarComponent::UpdateHealth);
	}

	Super::EndPlay(EndPlayReason);
}

void UHealthBarComponent::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
