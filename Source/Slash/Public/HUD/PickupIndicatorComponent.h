// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PickupIndicatorComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UPickupIndicatorComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY();
	TObjectPtr<class UPickupIndicator> PickupIndicatorWidget;

	bool bOverlapPlayer = false;
	bool bEquipped = false;

private:
	UFUNCTION()
	void OnEquipped();
	
	UFUNCTION()
	void OnDropped();

	UFUNCTION()
	void OnBeginOverlapPlayer(APawn* Pawn, APlayerController* PlayerController);

	UFUNCTION()
	void OnEndOverlapPlayer(APawn* Pawn, APlayerController* PlayerController);

	UFUNCTION()
	void UpdateVisibility();
};
