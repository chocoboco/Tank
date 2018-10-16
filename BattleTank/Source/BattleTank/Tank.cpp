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
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// 코드가 더 나을 것 같기도 하지만 컴파일 없이 변경 가능한 블루프린트가 더 나은거려나..
	//TankTurretComponent = Cast<UTankTurret>(GetComponentByClass(UTankTurret::StaticClass()));
	//if (TankTurretComponent)
	//{

	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("TankTurretComponent Invalid %s"), *GetName());
	//}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::SetTurretReference(UTankTurret* TurretToSet)
{
	TankAimingComponent->SetTurretReference( TurretToSet );
}

void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
	TankAimingComponent->SetBarrelReference(BarrelToSet);
}

bool ATank::IsCanAim() const
{
	return TankAimingComponent->IsCanAim();
}

void ATank::Fire()
{
	if (Barrel)
	{
		bool isReloaded = ReloadTimeInSeconds < (FPlatformTime::Seconds() - LastFireTime);
		if (isReloaded)
		{
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketTransform(FName("Projectile")));
			if (Projectile)
			{
				Projectile->LaunchProjectile(LaunchSpeed);
				LastFireTime = static_cast<float>(FPlatformTime::Seconds());
			}
		}
	}
}

void ATank::AimAt(FVector HitLocation)
{
	TankAimingComponent->AimAt( HitLocation, LaunchSpeed );
}

