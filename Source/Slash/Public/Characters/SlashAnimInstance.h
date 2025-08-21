// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <Characters/CharacterType.h>
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
public:
	/* Setter for UpperBodyOnly.
	* Whether animation montage blending only effect upper body.
	@Note animation montage must have UpperBody slot in default group.
	*/
	FORCEINLINE void SetUpperBodyOnly(bool Value) { UpperBodyOnly = Value; }
protected:
	/* Whether animation montage blending only effect upper body.
	@Note animation montage must have UpperBody slot in default group.
	*/
	UPROPERTY(BlueprintReadWrite)
	bool UpperBodyOnly;
};
