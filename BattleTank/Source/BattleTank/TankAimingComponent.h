// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
};

class ATank;
class UTankTurret;
class UTankBarrel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initalize( UTankBarrel* BarrelToSet, UTankTurret* TurretToSet );

	bool IsCanAim() const;

protected:
	void MoveBarrelTowards( FVector AimDirection );

public:	
	void AimAt(FVector WorldspaceAim, float LaunchSpeed);
	void SetFiringState( EFiringState NewState );
	EFiringState GetFiringState() const;

protected:
	ATank* OwnerTank = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringState FiringState = EFiringState::Reloading;
	
private:
	UTankTurret* Turret = nullptr;
	UTankBarrel* Barrel = nullptr;
	bool CurrentCanAim = false;
};
