// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ATank::IsCanAim() const
{
	if (TankAimingComponent)
		return TankAimingComponent->IsCanAim();
	else
		return false;
}

bool ATank::IsReloadEnd() const
{
	return ReloadTimeInSeconds < (FPlatformTime::Seconds() - LastFireTime);
}

void ATank::Fire()
{
	if (BarrelComponent)
	{
		bool isReloaded = IsReloadEnd();
		if (isReloaded)
		{
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, BarrelComponent->GetSocketTransform(FName("Projectile")));
			if (Projectile)
			{
				Projectile->LaunchProjectile(LaunchSpeed);
				LastFireTime = static_cast<float>(FPlatformTime::Seconds());

				if (TankAimingComponent)
					TankAimingComponent->SetFiringState(EFiringState::Reloading);
			}
		}
	}
}

void ATank::AimAt(FVector HitLocation)
{
	if (TankAimingComponent)
		TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}