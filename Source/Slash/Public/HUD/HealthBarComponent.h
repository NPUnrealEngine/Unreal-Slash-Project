// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	//void SetHealthPercent(float Percent);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	virtual void UpdateHealth(float CurrentHealth, float MaxHealth);

private:
	UPROPERTY()
	TObjectPtr<class UHealthBar> HealthBarWidget;
};
