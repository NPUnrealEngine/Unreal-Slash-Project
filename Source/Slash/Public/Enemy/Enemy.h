// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Slash/Public/Characters/BaseCharacter.h>
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

#pragma region AActor_override
	/** AActor */

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** AActor */
#pragma endregion AActor_override
	
#pragma region IHitInterface
	/** IHitInterface */

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
	/** IHitInterface */
#pragma endregion IHitInterface

#pragma region ABaseCharacter_override

	virtual void HandleDamage(float DamageAmount) override;

#pragma endregion ABaseCharacter_override
	
protected:
#pragma region AEmeny

	UPROPERTY(EditAnywhere, Category = "Enemy | Properties")
	float DeathLifeSpan = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

#pragma endregion AEmeny

#pragma region AActor_override
	/** AActor */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** AActor */
#pragma endregion AActor_override

#pragma region Debug

	void DrawDebugNavPathPoints(TArray<FNavPathPoint>& PathPoints);

#pragma endregion Debug

#pragma region ABaseCharacter_override
	/** ABaseCharacter */

	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;

	/** ABaseCharacter */
#pragma endregion ABaseCharacter_override
	
private:
#pragma region AI_Behavior
	/** AI Behavior Properties */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPawnSensingComponent> PawnSensing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHealthBarComponent> HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AWeapon> WeaponClass;

	/* Enemy AI controller */
	TObjectPtr<class AAIController> EnemyController;
	
	/* Current patrol target */
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	/* All patrol targets */
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "AI Behavior | Properties")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "AI Behavior | Properties")
	float ChasingSpeed = 300.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat | Properties")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat | Properties")
	float AttackMax = 1.f;

	/** AI Behavior Properties */
#pragma endregion AI_Behavior

private:
#pragma region AI_Behavior
	/* AI Behavior */

	void InitializeEmeny();
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void CheckPatrolTarget();
	void CheckCombatTarget();
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
	void StartAttackTimer();
	void PatrolTimerFinished();
	

	/* AI Behavior */
#pragma endregion AI_Behavior

#pragma region Weapon

	void SpawnDefaultWeapon();

#pragma endregion Weapon

};
