// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Characters/CharacterType.h>
#include <Slash/Public/Characters/BaseCharacter.h>
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void Destroyed() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(EditAnywhere, Category = "Enemy | Properties")
	float DeathLifeSpan = 10.f;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DrawDebugNavPathPoints(TArray<FNavPathPoint>& PathPoints);
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	/*
	* Play animation montage
	*/
	virtual int32 PlayDeathMontage() override;

private:
	/* 
	* Components
	*/

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 160.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> WeaponClass;

	/**
	* Navigation
	*/

	/* Enemy AI controller */
	TObjectPtr<class AAIController> EnemyController;
	
	/* Current patrol target*/
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	/* All patrol targets */
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "AI Behavior | Properties")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "AI Behavior | Properties")
	float ChasingSpeed = 300.f;

	/*
	* Combat
	*/

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat | Properties")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat | Properties")
	float AttackMax = 1.f;

private:
	void PatrolTimerFinished();

	/*
	* AI behavior
	*/

	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void ClearAttackTimer();

	/*
	* Combat behavior
	*/

	void StartAttackTimer();
};
