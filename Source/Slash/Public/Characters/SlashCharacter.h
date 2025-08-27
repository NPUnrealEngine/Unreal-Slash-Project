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

#pragma region AActor_override

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion AActor_override

#pragma region IHitInterface
	/** IHitInterface */

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	/** IHitInterface */
#pragma endregion IHitInterface


public:
#pragma region Setter

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

#pragma endregion Setter

#pragma region Getter

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

#pragma endregion Getter

protected:
#pragma region Input

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

#pragma endregion Input

protected:
#pragma region AActor_override

	virtual void BeginPlay() override;

#pragma endregion AActor_override

#pragma region ABaseCharacter_override

	virtual void Attack() override;

#pragma endregion ABaseCharacter_override

	
#pragma region Input_callback

	/* Callback move forward for old input system */
	//void MoveForward(float Value);

	/* Callback functions for enhanced input system */

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void EKeyPressed();
	void Dodge();

#pragma endregion Input_callback

	/* Set character state by weapon type*/
	void SetCharacterStateByWeaponType(EWeaponType WeaponType);

	/*
	* Play montage functions
	*/

	/* Callback for animation blueprint */
	
#pragma region Animation

	void PlayEquipMontage(FName SectionName);

	/* Enable xix upper body and lower body 
	while playing animation montage */
	void SetMixedBodyAnimEnabled(bool Enabled);

#pragma endregion Animation

	
#pragma region Combat

	void EquipWeapon(AWeapon* Weapon);
	void UnequipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	void Disarm();
	bool CanArm();
	void Arm();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	/* Callback for animation blueprint */
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

#pragma endregion Combat


#pragma region Equipped_item

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

#pragma endregion Equipped_item

private:
#pragma region Character_properties

	/* Character state */
	UPROPERTY(VisibleAnywhere, Category = "Character state")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	/* Character's action state */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Character state")
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

#pragma endregion Character_properties

#pragma region Detect_item

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

#pragma endregion Detect_item

	/*
	* Animation montage
	*/

	// Old method
	//UPROPERTY(EditDefaultsOnly, Category = "Montages")
	//TObjectPtr<UAnimMontage> AttackMontage;

	// Old method
	//UPROPERTY(EditDefaultsOnly, Category = "Montages")
	//TObjectPtr<UAnimMontage> Attack2HandMontage;

#pragma region Animation

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Montage")
	TObjectPtr<UAnimMontage> EquipMontage;

#pragma endregion Animation
};
