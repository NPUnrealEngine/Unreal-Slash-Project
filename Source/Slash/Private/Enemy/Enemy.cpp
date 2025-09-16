// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include <Components/SkeletalMeshComponent.h>
#include <Slash/DebugMacros.h>
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include <Components/AttributeComponent.h>
#include <HUD/HealthBarComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <AIController.h>
#include <Navigation/PathFollowingComponent.h>
#include <Perception/PawnSensingComponent.h>
#include <Items/Weapons/Weapon.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, 
		ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, 
		ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(40.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Drop(GetActorLocation());
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_COLOR_SPHERE_DURATION_RADIUS(ImpactPoint, FColor::Orange, 5.f, 8.f);

	Super::GetHit_Implementation(ImpactPoint);
	if(!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearPatrolTimer();
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	StopAttackMontage();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	/* Parent handle damage will borcast event */
	/*if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}*/
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	InitializeEmeny();
	Tags.Add(FName("Enemy"));
}

void AEnemy::DrawDebugNavPathPoints(TArray<FNavPathPoint>& PathPoints)
{
	for (int32 i = 0; i < PathPoints.Num(); i++)
	{
		const FVector& Location = PathPoints[i].Location;
		DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);

		if (i != 0)
		{
			const FVector& StartPoint = PathPoints[i - 1].Location;
			const FVector& EndPoint = PathPoints[i].Location;
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 12.f, 0, 3.f);
		}
	}
}

void AEnemy::Die()
{
	Super::Die();

	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	HideHealthBar();
	GetMesh()->SetGenerateOverlapEvents(false);
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance && DeathMontage)
	//{
	//	/* Play random section in montage without knowning section name ahead */
	//	AnimInstance->Montage_Play(DeathMontage);
	//	const int32 NumSections = DeathMontage->GetNumSections();
	//	const int32 Selection = FMath::RandRange(0, NumSections - 1);
	//	FName SectionName = DeathMontage->GetSectionName(Selection);

	//	switch (Selection)
	//	{
	//	case 0:
	//		DeathPose = EDeathPose::EDP_Death1;
	//		break;
	//	case 1:
	//		DeathPose = EDeathPose::EDP_Death2;
	//		break;
	//	case 2:
	//		DeathPose = EDeathPose::EDP_Death3;
	//		break;
	//	case 3:
	//		DeathPose = EDeathPose::EDP_Death4;
	//		break;
	//	case 4:
	//		DeathPose = EDeathPose::EDP_Death5;
	//		break;
	//	case 5:
	//		DeathPose = EDeathPose::EDP_Death6;
	//		break;
	//	default:
	//		break;
	//	}
	//	AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	//}

	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;

	EnemyState = EEnemyState::EES_Engaged;
	if (EquippedWeapon)
	{
		PlayAttackMontage();
	}
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::InitializeEmeny()
{
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState < EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Chasing &&
		SeenPawn->ActorHasTag(FName("EngagableTarget")) &&
		!SeenPawn->ActorHasTag(FName("Dead"));

	if (bShouldChaseTarget)
	{
		ClearPatrolTimer();
		CombatTarget = SeenPawn;
		ChaseTarget();
		//UE_LOG(LogTemp, Warning, TEXT("Pawn seen, chase character"));
	}
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Length();
	/*DRAW_SPHERE_SINGLE_FRAME(GetActorLocation());
	DRAW_SPHERE_SINGLE_FRAME(Target->GetActorLocation());*/
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<TObjectPtr<AActor>> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	if (ValidTargets.Num() > 0)
	{
		const int32 PatrolIndex = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[PatrolIndex];
	}

	return nullptr;
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		// Outside of combat, patrolling
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged())
		{
			StartPatrolling();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Lose interest"));
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		// Outside of attack, chase character
		ClearAttackTimer();
		if (!IsEngaged())
		{
			ChaseTarget();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Chase character"));
	}
	else if (CanAttack())
	{
		// Inside attacking range, attack character
		ClearAttackTimer();
		StartAttackTimer();
		//UE_LOG(LogTemp, Warning, TEXT("Attack character"));
	}
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}