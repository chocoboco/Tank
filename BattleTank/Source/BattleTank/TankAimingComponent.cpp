// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::Initalize( UTankBarrel* BarrelToSet, UTankTurret* TurretToSet )
{
	if (BarrelToSet && TurretToSet)
	{
		Barrel = BarrelToSet;
		Turret = TurretToSet;
	}
}

bool UTankAimingComponent::IsCanAim() const
{
	return CurrentCanAim;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (Barrel)
	{
		FVector OutLaunchVelocity;
		FVector StartLocation = Barrel->GetSocketLocation( FName("Projectile") );
		TArray<AActor*> ActorToIgnore;

		CurrentCanAim = UGameplayStatics::SuggestProjectileVelocity(this,
			/*out*/ OutLaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
			false,
			0.0f,
			0.0f,
			ESuggestProjVelocityTraceOption::DoNotTrace,
			FCollisionResponseParams::DefaultResponseParam, 
			ActorToIgnore,
			false );

		if (CurrentCanAim)
		{
			auto AimDirection = OutLaunchVelocity.GetSafeNormal();
			MoveBarrelTowards( AimDirection );
			SetFiringState( EFiringState::Aiming );

			//UE_LOG(LogTemp, Warning, TEXT("Solution Found. %s aiming at %s from %s speed(%f)"), *GetName(), *HitLocation.ToString(), *Barrel->GetComponentLocation().ToString(), LaunchSpeed);
		}
		else
		{
			SetFiringState( EFiringState::Locked );
			//UE_LOG(LogTemp, Warning, TEXT("Solution Not Found. %s"), *GetName() );
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s speed(%f)"), *GetName(), *HitLocation.ToString(), *Barrel->GetComponentLocation().ToString(), LaunchSpeed);
}

void UTankAimingComponent::Fire()
{
	if (Barrel)
	{
		ensure( ProjectileBlueprint );

		bool isReloaded = IsReloadEnd();
		if (isReloaded)
		{
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketTransform(FName("Projectile")));
			if (Projectile)
			{
				Projectile->LaunchProjectile(LaunchSpeed);
				LastFireTime = static_cast<float>(FPlatformTime::Seconds());

				SetFiringState(EFiringState::Reloading);
			}
		}
	}
}

bool UTankAimingComponent::IsReloadEnd() const
{
	return ReloadTimeInSeconds < (FPlatformTime::Seconds() - LastFireTime);
}

void UTankAimingComponent::SetFiringState(EFiringState NewState)
{
	if( (FiringState == EFiringState::Reloading) &&
		(NewState == EFiringState::Aiming || NewState == EFiringState::Locked) )
	{
		if (IsReloadEnd() == false)
			return;
	}

	//UE_LOG( LogTemp, Warning, TEXT("SetFiringState: %d"), (int)NewState );
	FiringState = NewState;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;
	
	if (180.0f < FMath::Abs(DeltaRotator.Yaw))
	{
		if (DeltaRotator.Yaw < 0.0f)
			DeltaRotator.Yaw += 360.0f;
		else
			DeltaRotator.Yaw -= 360.0f;
	}

	Turret->Rotate( DeltaRotator.Yaw );
	Barrel->Elevate( DeltaRotator.Pitch );
}

void UTankAimingComponent::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{

}