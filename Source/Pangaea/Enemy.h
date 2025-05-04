// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PangaeaCharacter.h"
#include "Weapon.h"
#include "Enemy.generated.h"

UCLASS()
class PANGAEA_API AEnemy : public APangaeaCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APawn* _ChasedTarget = nullptr;
	UClass* _WeaponClass;
	AWeapon* _Weapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack() override;
	void DieProcess() override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	void Chase(APawn* targetPawn);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;
};
