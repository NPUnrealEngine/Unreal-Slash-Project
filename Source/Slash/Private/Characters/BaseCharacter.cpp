// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/AttributeComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Items/Weapons/Weapon.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>


ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
		ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::AttackEnd()
{
}

int32 ABaseCharacter::PlayAttackMontage()
{
	// Get the attack montage associate with character state
	UAnimMontage* SelectedMontage = AttackMontageMap.FindChecked(EquippedWeapon->GetWeaponType());
	if (!SelectedMontage) return -1;

	// Pick a section within montage
	const int32 Selection = RandomMontageSection(SelectedMontage);
	if (Selection < 0) return Selection;

	FName SectionName = SelectedMontage->GetSectionName(Selection);

	PlayMontageSection(SelectedMontage, SectionName);

	return Selection;
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = RandomMontageSection(DeathMontage);
	if (Selection < 0) return Selection;

	FName SectionName = DeathMontage->GetSectionName(Selection);

	PlayMontageSection(DeathMontage, SectionName);

	return Selection;
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();

	// Lower ImpactPoint's z to actor's z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	/*
	* Find out degrees between two vectors
	*/
	// ForwardVector * ToHit = |ForwardVector| X |ToHit| X COS(theta), where * = dot product and X = multiply
	// When |ForwardVector| = 1, |ToHit| = 1 Normalized, then ForwardVector * ToHit = COS(theta) 
	const double CosTheta = FVector::DotProduct(ForwardVector, ToHit);

	// Take the inverse cos (arc-cosine) of cos(theta) to get theta
	double theta = FMath::Acos(CosTheta);

	// Convert radians to degrees
	theta = FMath::RadiansToDegrees(theta);

	// If CrossProduct poiting down, theta should be negative otherwise
	// theta is positive
	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToHit);
	if (CrossProduct.Z < 0)
	{
		theta *= -1.f;
	}

	FName SectionName("FromBack");

	if (theta >= -45.f && theta < 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (theta >= -135.f && theta < -45.f)
	{
		SectionName = FName("FromLeft");
	}
	else if (theta >= 45.f && theta < 135.f)
	{
		SectionName = FName("FromRight");
	}

	PlayHitReactMontage(SectionName);

	/* Print theta to screen */
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f,
			FColor::Green,
			FString::Printf(TEXT("Theta: %f"), theta));
	}*/

	/* Draw debug arrow */
	//UKismetSystemLibrary::DrawDebugArrow(this,
	//	GetActorLocation(),
	//	GetActorLocation() + CrossProduct * 100.f,
	//	5.f,
	//	FColor::Blue,
	//	5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this,
	//	GetActorLocation(),
	//	GetActorLocation() + ForwardVector * 60.f,
	//	5.f,
	//	FColor::Red,
	//	5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this,
	//	GetActorLocation(),
	//	GetActorLocation() + ToHit * 60.f,
	//	5.f,
	//	FColor::Green,
	//	5.f);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticle(const FVector& ImpactPoint)
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

const int32 ABaseCharacter::RandomMontageSection(UAnimMontage* Montage)
{
	const int32 NumSections = Montage->GetNumSections();
	if (NumSections <= 0) return -1;

	const int32 Selection = FMath::RandRange(0, NumSections - 1);

	return Selection;
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

