// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaCharacter.h"
#include "PangaeaAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

#include <Net/UnrealNetwork.h>

APangaeaCharacter::APangaeaCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
}

void APangaeaCharacter::BeginPlay()
{
    Super::BeginPlay();

    _AnimInstance = Cast<UPangaeaAnimInstance>(GetMesh()->GetAnimInstance());
    _HealthPoints = HealthPoints;
    //체력바 업데이트 코드
    if (HealthBarWidget != nullptr)
    {
        float normalizeHealth = FMath::Clamp(
            (float)_HealthPoints / HealthPoints, 0.0f, 1.0f);
        auto healthBar = Cast<UHealthBarWidget>(HealthBarWidget);
        healthBar->HealthProgressBar->SetPercent(normalizeHealth);
    }
}

void APangaeaCharacter::Attack_Broadcast_RPC_Implementation()
{
    Attack();
}

void APangaeaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APangaeaCharacter, _HealthPoints);
}

void APangaeaCharacter::OnRep_OnHealthPointsChanged()
{
    //체력바 업데이트 코드
    if (HealthBarWidget != nullptr)
    {
        float normalizeHealth = FMath::Clamp(
            (float)_HealthPoints / HealthPoints, 0.0f, 1.0f);
        auto healthBar = Cast<UHealthBarWidget>(HealthBarWidget);
        healthBar->HealthProgressBar->SetPercent(normalizeHealth);
    }

    if (_AnimInstance != nullptr)
    {
        _AnimInstance->State = ECharacterState::Hit;
    }

    if (IsKilled())
    {
        PrimaryActorTick.bCanEverTick = false;
    }
}

void APangaeaCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

int APangaeaCharacter::GetHealthPoints()
{
    return _HealthPoints;
}

bool APangaeaCharacter::IsKilled()
{
    return (_HealthPoints <= 0.0f);
}

bool APangaeaCharacter::CanAttack()
{
    return (_AttackCountingDown <= 0.0f && _AnimInstance->State == ECharacterState::Locomotion);
}

void APangaeaCharacter::Attack()
{
    _AttackCountingDown = AttackInterval;
}

void APangaeaCharacter::Hit(int damage)
{
    if (IsKilled())
    {
        return;
    }

    if (GetNetMode() == NM_ListenServer && HasAuthority())
    {
        _HealthPoints -= damage;
        OnRep_OnHealthPointsChanged();
    }
}

void APangaeaCharacter::DieProcess()
{
    PrimaryActorTick.bCanEverTick = false;
    Destroy();
    GEngine->ForceGarbageCollection(true);
}