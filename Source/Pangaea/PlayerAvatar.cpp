// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatar.h"

// Sets default values
APlayerAvatar::APlayerAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//카메라 스프링암 생성
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArmComponent->SetupAttachment(RootComponent);
	_springArmComponent->SetUsingAbsoluteRotation(true);
	_springArmComponent->TargetArmLength = 800.f;
	_springArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	_springArmComponent->bDoCollisionTest = false;

	

	//카메라 생성
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
	_cameraComponent->bUsePawnControlRotation = false;

	//게임 프레임이 업데이트될 때마다 캐릭터의 Tick 함수를 호출할 수 있도록 함
	PrimaryActorTick.bCanEverTick = true;

	//내려보기 형식이므로 캐릭터가 달리는 방향만 결정하면 됨 (캐릭터가 회전할 필요 없음)
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	auto* characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	characterMovement->bConstrainToPlane = true;
	characterMovement->bSnapToPlaneAtStart = true;
	
}

// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int APlayerAvatar::GetHealthPoints()
{
	return 0;
}

bool APlayerAvatar::IsKilled()
{
	return false;
}

bool APlayerAvatar::CanAttack()
{
	return false;
}

