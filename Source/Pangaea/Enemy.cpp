// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyController.h"
#include "PangaeaAnimInstance.h"

// Sets default values
AEnemy::AEnemy()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(
		TEXT("Blueprint'/Game/TopDown/Blueprints/BP_Hammer.BP_Hammer'"));

	_WeaponClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(_WeaponClass));
	_Weapon->Holder = this;
	_Weapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		FName("hand_rSocket"));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();

	if (_AttackCountingDown == AttackInterval)
	{
		_AnimInstance->State = ECharacterState::Attack;
	}

	if (_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}

	if (_ChasedTarget != nullptr &&
		_AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(_ChasedTarget);
	}
}

void AEnemy::Chase(APawn* targetPawn)
{
	if (targetPawn != nullptr && _AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MoveToActor(targetPawn, 90.0f);
	}

	_ChasedTarget = targetPawn;
}

void AEnemy::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	K2_DestroyActor();
	GEngine->ForceGarbageCollection(true);
}

void AEnemy::Attack()
{
	Super::Attack();
	GetController()->StopMovement();
}