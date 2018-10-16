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

class UTankTurret;
class UTankBarrel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	void SetTurretReference(UTankTurret* TurretToSet);
	void SetBarrelReference(UTankBarrel* BarrelToSet);

	bool IsCanAim() const;

protected:
	void MoveBarrelTowards( FVector AimDirection );

public:	
	void AimAt(FVector WorldspaceAim, float LaunchSpeed);

protected:
	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringState FiringState = EFiringState::Reloading;
	
private:
	UTankTurret* Turret = nullptr;
	UTankBarrel* Barrel = nullptr;
	bool CurrentCanAim = false;
};
