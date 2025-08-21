// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Slash/Public/Interfaces/HitInterface.h>
#include "BreakableActor.generated.h"

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bBroken = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Breakable | Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	UPROPERTY(EditAnywhere, Category = "Breakable | Properties")
	float TreasureSpawnZOffset = 75.f;
};
