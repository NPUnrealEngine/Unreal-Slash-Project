// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/*
	 Bird Input Mapping Context
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* BirdMappingContext;

	/*
	 Enhanced Input System, Input action move
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/*
	 Enhanced Input System, Input action mouse look
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	virtual void BeginPlay() override;

	/**
	 Movement callback for Old Input system 
	*/
	void MoveForward(float Value);

	/*
	 Turn(left/right) for callback Old Input system
	*/
	void Turn(float Value);

	/*
	 LookUp(up/down) callback for Old Input system
	*/
	void LookUp(float Value);

	/**
	 Movement callback for Enhanced Input sytem to bind
	*/
	void Move(const FInputActionValue& Value);

	/**
	 Mouse look callback for Enhanced Input sytem to bind
	*/
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
