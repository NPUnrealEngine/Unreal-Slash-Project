// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 NumberOfGold);

public:
	FORCEINLINE float GetCurrentHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetStaminaDelayRegen() const { return DelayStaminaRegen; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/* Current health */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100.f;

	/* Max health */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100.f;

	/* Current stamina */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina = 100.f;

	/* Max stamina */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina = 100.f;

	/* Gold count */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold = 0;

	/* Soul count */
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls = 0;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DelayStaminaRegen = 1.5f;
};
