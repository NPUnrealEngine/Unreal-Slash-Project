// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include <Characters/SlashCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>
#include <Components//BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Slash/Public/Interfaces/HitInterface.h>
#include <NiagaraComponent.h>

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponBox->SetGenerateOverlapEvents(true);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxOverlap);

	if (WeaponTrailEffect)
	{
		WeaponTrailComponent = UGameplayStatics::SpawnEmitterAttached(
			WeaponTrailEffect,
			GetRootComponent(),
			FName(""),
			FVector::Zero(),
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false
		);
	}
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	PlayEquipSound();

	DisableSphereCollision();

	DeactivateEmber();
}

void AWeapon::Drop(FVector Location)
{
	ItemState = EItemState::EIS_Hovering;
	SetOwner(nullptr);
	SetInstigator(nullptr);

	ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	this->SetActorLocation(Location);
	this->SetActorRotation(FRotator::ZeroRotator);

	EnableSphereCollision();

	ActivateEmber();
}

void AWeapon::DeactivateEmber()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

void AWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

void AWeapon::EnableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AWeapon::ActivateEmber()
{
	if (ItemEffect)
	{
		ItemEffect->Activate();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	// Attach weapon to character's skeletal mesh's specific socket
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::StartTrail()
{
	if (WeaponTrailComponent)
	{
		WeaponTrailComponent->BeginTrails(
			FName("TrailStartSocket"), 
			FName("TrailEndSocket"), 
			ETrailWidthMode::ETrailWidthMode_FromFirst,
			WeaponTrailWidth
		);
	}
}

void AWeapon::EndTrail()
{
	if (WeaponTrailComponent)
	{
		WeaponTrailComponent->EndTrails();
	}
}

void AWeapon::OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(FName("Enemy")) && OtherActor->ActorHasTag("Enemy")) return;
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		if (GetOwner()->ActorHasTag(FName("Enemy")) && BoxHit.GetActor()->ActorHasTag("Enemy")) return;

		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);
		ExecuteGetHit(BoxHit);
		CreateField(BoxHit.ImpactPoint);
	}
	
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(
			WeaponBox->GetUnscaledBoxExtent().X,
			WeaponBox->GetUnscaledBoxExtent().Y,
			1.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowDebugBox? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.0f
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
