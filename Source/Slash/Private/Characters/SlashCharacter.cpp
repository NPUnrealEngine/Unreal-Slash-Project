// Fill out your copyright notice in the Description page of Project Settings.

#include <Characters/SlashCharacter.h>
#include <Characters/SlashAnimInstance.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <GroomComponent.h>
#include <Components/AttributeComponent.h>

#include <InputMappingContext.h>
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

#include <Items/Weapons/Weapon.h>

#include <HUD/SlashHUD.h>
#include <HUD/SlashOverlay.h>


ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 900.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Visibility, 
		ECollisionResponse::ECR_Block
	);
	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_WorldDynamic,
		ECollisionResponse::ECR_Overlap
	);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");

}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngagableTarget"));

	InitializeSlashOverlay();

	SetupPlayerInput();
}

void ASlashCharacter::SetupPlayerInput()
{
	/* Offical implementation */
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (!SlashContext.IsNull())
			{
				InputSubsystem->ClearAllMappings();
				InputSubsystem->AddMappingContext(SlashContext.LoadSynchronous(), 0);
			}
		}
	}

	/* Same as above but different implementation */
	/*if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->AddMappingContext(SlashContext, 0);
	}*/
}

//void ASlashCharacter::MoveForward(float Value)
//{
//	if (GetController() && Value != 0.f) 
//	{
//		FVector Forward = GetActorForwardVector();
//		AddMovementInput(Forward, Value);
//	}
//}

void ASlashCharacter::Move(const FInputActionValue& Value) 
{
	/* Only attacking action can't move */
	if (ActionState == EActionState::EAS_Attacking) return;

	/* Any actions can't move */
	//if (ActionState != EActionState::EAS_Unoccupied) return;

	/*  Following implementation for character movement
		is base on camera facing direction
	*/
	if (GetController()) 
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		// Get current controller rotation
		const FRotator ControllerRotation = GetControlRotation();

		// Make a rotation only for yaw(z axis), which came
		// from controller rotation. Pitch and Roll are ignored.
		const FRotator ControllerYawRotation(0.f, ControllerRotation.Yaw, 0.f);

		// Get forward unit vector
		const FVector ForwardVector = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);

		// Move character along forward vector
		AddMovementInput(ForwardVector, MovementVector.Y);

		// Get right unit vector
		const FVector RightVector = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);
		
		// Move character along right vector 
		AddMovementInput(RightVector, MovementVector.X);
	}

	/* Simple movement
	   Following method only move character base on
	   player input without considering camera direction
	*/
	/*const FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector ForwardVector = GetActorForwardVector();
	const FVector RightVector = GetActorRightVector();

	AddMovementInput(ForwardVector, MovementVector.Y);
	AddMovementInput(RightVector, MovementVector.X);*/
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Add pitch to controller
	AddControllerPitchInput(LookAxisVector.Y);

	// Add yaw to controller
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			UnequipWeapon(EquippedWeapon);
		}

		if (EquippedWeapon == nullptr) EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_Unequipping;

	//UE_LOG(LogTemp, Warning, TEXT("Disarm"));
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && 
		EquippedWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	SetCharacterStateByWeaponType(EquippedWeapon->GetWeaponType());
	ActionState = EActionState::EAS_Equipping;

	//UE_LOG(LogTemp, Warning, TEXT("Arm"));
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::FinishUnequipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::StartTrail()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartTrail();
	}
}

void ASlashCharacter::EndTrail()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->EndTrail();
	}
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/* Hard code attack montage */
//void ASlashCharacter::PlayAttackMontage()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//
//	if (USlashAnimInstance* SlashAnimInstance = Cast<USlashAnimInstance>(AnimInstance)) {
//		SlashAnimInstance->SetUpperBodyOnly(false);
//	}
//	if (AnimInstance && AttackMontage)
//	{
//		AnimInstance->Montage_Play(AttackMontage);
//		const int32 Selection = FMath::RandRange(0, 3);
//		FName SectionName = FName();
//
//		switch (Selection)
//		{
//		case 0:
//			SectionName = FName("Attack1");
//			break;
//		case 1:
//			SectionName = FName("Attack2");
//			break;
//		case 2:
//			SectionName = FName("Attack3");
//			break;
//		case 3:
//			SectionName = FName("Attack4");
//			break;
//		default:
//			break;
//		}
//
//		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
//
//		/* Play random section in montage without knowning section name ahead */
//		/*AnimInstance->Montage_Play(AttackMontage);
//		const int32 NumSections = AttackMontage->GetNumSections();
//		const int32 Selection = FMath::RandRange(0, NumSections-1);
//		FName SectionName = AttackMontage->GetSectionName(Selection);
//		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);*/
//	}
//}

//void ASlashCharacter::PlayAttackMontageByState(ECharacterState State)
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//
//	if (USlashAnimInstance* SlashAnimInstance = Cast<USlashAnimInstance>(AnimInstance)) {
//		SlashAnimInstance->SetUpperBodyOnly(false);
//	}
//
//	// Get the attack montage associate with character state
//	UAnimMontage* SelectedMontage = AttackMontageMap.FindChecked(CharacterState);
//	if (AnimInstance && SelectedMontage)
//	{
//		AnimInstance->Montage_Play(SelectedMontage);
//		const int32 NumSections = SelectedMontage->GetNumSections();
//		const int32 Selection = FMath::RandRange(0, NumSections - 1);
//		FName SectionName = SelectedMontage->GetSectionName(Selection);
//		AnimInstance->Montage_JumpToSection(SectionName, SelectedMontage);
//	}
//}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	SetMixedBodyAnimEnabled(true);
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::SetMixedBodyAnimEnabled(bool Enabled)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (USlashAnimInstance* SlashAnimInstance = Cast<USlashAnimInstance>(AnimInstance)) {
		SlashAnimInstance->SetUpperBodyOnly(Enabled);
	}
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	SetCharacterStateByWeaponType(Weapon->GetWeaponType());
	EquippedWeapon = Weapon;
	OverlappingItem = nullptr;
	//CharacterState = ECharacterState::ECS_EquipTwoHandWeapon;
	//UE_LOG(LogTemp, Warning, TEXT("Equip weapon"));
}

void ASlashCharacter::UnequipWeapon(AWeapon* Weapon)
{
	Weapon->Drop(OverlappingItem->GetActorLocation());
	OverlappingItem = nullptr;
	EquippedWeapon = nullptr;
	CharacterState = ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		if (EquippedWeapon)
		{
			SetMixedBodyAnimEnabled(false);
			PlayAttackMontage();
			ActionState = EActionState::EAS_Attacking;
		}
	}
}

void ASlashCharacter::Dodge()
{
}

void ASlashCharacter::SetCharacterStateByWeaponType(EWeaponType WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponType::EWT_OneHand:
		CharacterState = ECharacterState::ECS_EquipOneHandWeapon;
		break;
	case EWeaponType::EWT_TwoHand:
		CharacterState = ECharacterState::ECS_EquipTwoHandWeapon;
		break;
	default:
		CharacterState = ECharacterState::ECS_EquipOneHandWeapon;
		break;
	}
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
	}

	/* Old input system callback binding */
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	ActionState = EActionState::EAS_HitReaction;
}

void ASlashCharacter::InitializeSlashOverlay()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD()))
		{
			if (SlashOverlay = SlashHUD->GetSlashOverlay())
			{
				if (Attributes) SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}



