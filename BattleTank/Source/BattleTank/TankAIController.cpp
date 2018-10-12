// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"


// Called when the game starts or when spawned
void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}



void ATankAIController::Tick(float DeltaTime)
{
	ATank* TargetPlayerTank = GetPlayerTank();
	if (TargetPlayerTank)
	{
		ATank* ControlledTank = GetControlledTank();
		ControlledTank->AimAt( TargetPlayerTank->GetActorLocation() );

		ControlledTank->Fire();
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerTank)
	{
		return PlayerTank;
	}
	else
	{
		return nullptr;
	}
}
