// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Private/KismetTraceUtils.h"


// Called when the game starts or when spawned
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG( LogTemp, Warning, TEXT("PlayerController possessing: %s"), *(ControlledTank->GetName()) );
	}
}


ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>( GetPawn() );
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();

	//UE_LOG( LogTemp, Warning, TEXT("PlayerController bCanEverTick: %d"), PrimaryActorTick.bCanEverTick );
}

void ATankPlayerController::AimTowardsCrosshair()
{
	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		FVector HitLocation;
		if( GetSightRayHitLocation(HitLocation) )
		{
			ControlledTank->AimAt( HitLocation );
			//UE_LOG( LogTemp, Warning, TEXT("Hit location: %s"), *HitLocation.ToString() );
		}
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize( ViewportSizeX, ViewportSizeY );

	auto ScreenLocation = FVector2D( ViewportSizeX*CrosshairXLocation, 
									 ViewportSizeY*CrosshairYLocation );

	FVector LookLocation;
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookLocation, LookDirection))
	{
		GetLookVectorHitLocation( LookDirection, /*OUT*/ HitLocation );
		DrawDebugDirectionalArrow(GetWorld(), LookLocation, LookLocation + (LookDirection*1000000.0f), 10000.0f, FColor::Red );
	}

	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& WorldLocation, FVector& LookDirection) const
{
	return DeprojectScreenPositionToWorld( ScreenLocation.X, 
										   ScreenLocation.Y,
										   WorldLocation,
										   LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult hitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
					hitResult, 
					StartLocation,
					EndLocation, 
					ECollisionChannel::ECC_Visibility))
	{
		HitLocation = hitResult.Location;
		DrawDebugBox( GetWorld(), HitLocation, FVector(10.0f, 10.0f, 10.0f), FColor::Red );
		return true;
	}

	HitLocation = FVector( 0.0f );
	return false;
}