// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class UTankBarrel;
class UTankTurret;
class AProjectile;


UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	UPROPERTY(BlueprintReadWrite)
	UTankAimingComponent* TankAimingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 4000.0f;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3.0f;

	float LastFireTime = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	UTankBarrel* BarrelComponent = nullptr;
	

protected:

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsReloadEnd() const;

	UFUNCTION(BlueprintCallable)
	bool IsCanAim() const;

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

	void AimAt( FVector HitLocation );
};
