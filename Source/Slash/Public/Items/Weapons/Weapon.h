// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include <Items/Weapons/WeaponType.h>
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	TArray<AActor*> IgnoreActors;
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmber();
	void DisableSphereCollision();
	void PlayEquipSound();
	void EnableSphereCollision();
	void ActivateEmber();
	void Drop(FVector Location);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	/* Start weapon trail */
	void StartTrail();

	/* End weapon trail */
	void EndTrail();

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

protected:
	UPROPERTY(EditAnywhere, Category = "Weapon Type")
	EWeaponType WeaponType = EWeaponType::EWT_OneHand;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Trail | Properties")
	TObjectPtr<UParticleSystem> WeaponTrailEffect;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Trail | Properties")
	TObjectPtr<UParticleSystemComponent> WeaponTrailComponent;

	UPROPERTY(EditAnywhere, Category = "Weapon Trail | Properties")
	float WeaponTrailWidth = 1.f;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ExecuteGetHit(FHitResult& BoxHit);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowDebugBox = false;

private:
	void BoxTrace(FHitResult& BoxHit);
};
