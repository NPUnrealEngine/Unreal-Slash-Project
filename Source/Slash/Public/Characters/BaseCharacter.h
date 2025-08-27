// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Slash/Public/Items/Weapons/WeaponType.h>
#include <Slash/Public/Interfaces/HitInterface.h>
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

public:
#pragma region AActor_override

	virtual void Tick(float DeltaTime) override;

#pragma endregion AActor_override

protected:
#pragma region Combat

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class AWeapon> EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAttributeComponent> Attributes;

#pragma endregion Combat

protected:
#pragma region AActor_override

	virtual void BeginPlay() override;

#pragma endregion AActor_override

#pragma region Combat

	virtual void Attack();
	virtual bool CanAttack();
	virtual void Die();
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	bool IsAlive();
	virtual void HandleDamage(float DamageAmount);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

#pragma endregion Combat

#pragma region VFX

	void SpawnHitParticle(const FVector& ImpactPoint);

#pragma endregion VFX

#pragma region SFX

	void PlayHitSound(const FVector& ImpactPoint);

#pragma endregion SFX

#pragma region Animation

	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	virtual const int32 RandomMontageSection(UAnimMontage* Montage);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void PlayHitReactMontage(const FName& SectionName);

#pragma endregion Animation

#pragma region Mic

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable);

	/* Disable character's capsule collision */
	void DisableCapsule();

#pragma endregion Mic

	

private:
#pragma region Animation

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	/* Attack montage associate character's weapon type */
	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TMap<EWeaponType, UAnimMontage*> AttackMontageMap;

#pragma endregion Animation

#pragma region SFX

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;

#pragma endregion SFX

#pragma region VFX

	UPROPERTY(EditAnywhere, Category = "Visual Effect")
	TObjectPtr<UParticleSystem> HitParticle;

#pragma endregion VFX

};
