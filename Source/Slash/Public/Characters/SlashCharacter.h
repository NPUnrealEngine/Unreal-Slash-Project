// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include <Characters/CharacterType.h>
#include <Characters/BaseCharacter.h>
#include <Items/Weapons/WeaponType.h>
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
protected:
	/* Slash character input mapping context */
	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> SlashContext;

	/* Movement input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MovementAction;

	/* Look input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/* Jump input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/* E key pressed input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EKeyAction;

	/* Attack input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	/* Dodge input action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;
protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;

	/* Callback move forward for old input system */
	//void MoveForward(float Value);

	/* Callback functions for enhanced input system */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void EKeyPressed();
	void Dodge();

	

	/* Set character state by weapon type*/
	void SetCharacterStateByWeaponType(EWeaponType WeaponType);

	/*
	* Play montage functions
	*/

	virtual void PlayAttackMontage() override;

	/* Callback for animation blueprint */
	virtual void AttackEnd() override;

	virtual bool CanAttack() override;

	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void Disarm();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void Arm();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void FinishUnequipping();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void StartTrail();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void EndTrail();

private:
	/* Character state */
	UPROPERTY(VisibleAnywhere)
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	/* Character's action state */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/* Spring Arm */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	/* Camera */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

	/* Hair */
	UPROPERTY(VisibleAnywhere, Category = "Groom | Hair")
	TObjectPtr<UGroomComponent> Hair;

	/* Eye brows*/
	UPROPERTY(VisibleAnywhere, Category = "Groom | EyeBrows")
	TObjectPtr<UGroomComponent> EyeBrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	

	/*
	* Animation montage
	*/

	// Old method
	//UPROPERTY(EditDefaultsOnly, Category = "Montages")
	//TObjectPtr<UAnimMontage> AttackMontage;

	// Old method
	//UPROPERTY(EditDefaultsOnly, Category = "Montages")
	//TObjectPtr<UAnimMontage> Attack2HandMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TObjectPtr<UAnimMontage> EquipMontage;

};
