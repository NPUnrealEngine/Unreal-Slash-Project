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
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}

	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}
}

void AWeapon::Drop(FVector Location)
{
	SetOwner(nullptr);
	SetInstigator(nullptr);
	ItemState = EItemState::EIS_Hovering;

	ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	this->SetActorLocation(Location);
	this->SetActorRotation(FRotator::ZeroRotator);

	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (EmbersEffect)
	{
		EmbersEffect->Activate();
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

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(actor);
	}

	FHitResult BoxHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		WeaponBox->GetScaledBoxExtent(),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	
	if (BoxHit.GetActor())
	{
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}

		IgnoreActors.AddUnique(BoxHit.GetActor());

		CreateField(BoxHit.ImpactPoint);
	}
}
