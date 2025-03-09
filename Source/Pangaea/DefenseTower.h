// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADefenseTower();

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int ShellDefense = 2;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float AttackRange = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float ReloadInterval = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int _HealthPoints;
	float _ReloadCountingDown;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Defense Tower", meta = (DisplayName = "GetHP"))
	int GetHealthPoints();

	UFUNCTION(BlueprintPure, Category = "Pangaea|Defense Tower")
	bool IsDestroyed();

	UFUNCTION(BlueprintPure, Category = "Pangaea|Defense Tower")
	bool CanFire();
	void Fire();
	void Hit(int damage);

	FORCEINLINE UBoxComponent* GetBoxComponent() const
	{
		return _BoxComponent;
	}

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

protected:
	void DestroyProcess();

private:
	UPROPERTY(VisibleAnywhere, Category = "Tower Component", meta = (AllowPrivateAcess = "true"))
	UBoxComponent* _BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Tower Component", meta = (AllowPrivateAcess = "true"))
	UStaticMeshComponent* _MeshComponent;

};
