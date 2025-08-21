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

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class AWeapon> EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAttributeComponent> Attributes;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "Visual Effect")
	TObjectPtr<UParticleSystem> HitParticle;

	/* Attack montage associate character's weapon type */
	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TMap<EWeaponType, UAnimMontage*> AttackMontageMap;



protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual bool CanAttack();
	virtual void Die();
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticle(const FVector& ImpactPoint);
	bool IsAlive();
	virtual void HandleDamage(float DamageAmount);
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	virtual const int32 RandomMontageSection(UAnimMontage* Montage);

	/* Disable character's capsule collision */
	void DisableCapsule();

	/*
	* Animation blueprint callback
	*/

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	/*
	* Play montage animation
	*/

	/* Play attack montage
	* It will play montage associate with weapon type then
	* play random selected section
	*/
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void PlayHitReactMontage(const FName& SectionName);


};
