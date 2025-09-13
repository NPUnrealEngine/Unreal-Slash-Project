// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include <Slash/DebugMacros.h>
#include <Characters/SlashCharacter.h>
#include "Components/SphereComponent.h"
#include <NiagaraComponent.h>
#include <Interfaces/PickupInterface.h>
#include <NiagaraFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>

#define THIRTY 30

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	/*int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("Average 1 and 3 is %d"), AvgInt);

	float AvgFloat = Avg<float>(3.45f, 7.86f);
	UE_LOG(LogTemp, Warning, TEXT("Average 3.45 and 7.86 is %f"), AvgFloat);*/
	
	/*UE_LOG(LogTemp, Warning, TEXT("Begin play called"));

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item on screen"));
	}*/

	/*SetActorLocation(FVector(0.f, 0.f, 100.f));
	SetActorRotation(FRotator(0.f, 45.f, 0.f));

	UWorld* world = GetWorld();
	FVector location = GetActorLocation();
	FVector forward = GetActorForwardVector();
	FVector up = GetActorUpVector();

	DRAW_LINE(location, location + forward * 100.f);
	DRAW_POINT(location + forward * 100.f);
	DRAW_VECTOR(location, location + forward * 100.f);
	DRAW_SPHERE(location);
	DRAW_COLOR_SPHERE(location, FColor::Red);
	DRAW_ARROW(location, location + up * 100.f);*/
}

float AItem::TransformSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformCosin()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

float AItem::RotateDelta(float DeltaTime)
{
	return RotateConstant * DeltaTime;
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*const FString Message = FString::Printf(TEXT("%s actor begin overlapped"), *OtherActor->GetName());
	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, Message);
	}*/

	IPickupInterface* Pickup = Cast<IPickupInterface>(OtherActor);
	if (Pickup)
	{
		Pickup->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*const FString Message = FString::Printf(TEXT("%s actor end overlap"), *OtherActor->GetName());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, Message);
	}*/

	IPickupInterface* Pickup = Cast<IPickupInterface>(OtherActor);
	if (Pickup)
	{
		Pickup->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupEffect()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering) 
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformSin()));
	}

	/*float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

	AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));*/
	
	/*float movementRate = 50.f;
	float rotationRate = 45.f;

	AddActorWorldOffset(FVector(movementRate * DeltaTime, 0.f, 0.f));
	AddActorWorldRotation(FRotator(0.f, rotationRate * DeltaTime, 0.f));*/
	/*DRAW_SPHERE_SINGLE_FRAME(GetActorLocation());
	DRAW_VECTOR_SINGLE_FRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);

	FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	DRAW_POINT_SINGLE_FRAME(AvgVector);*/

	/*UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);

	if (GEngine) {
		FString name = GetName();
		FString message = FString::Printf(TEXT("DeltaTime: %s"), *name);
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, message);

		UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %s"), *name);
	}*/
}

