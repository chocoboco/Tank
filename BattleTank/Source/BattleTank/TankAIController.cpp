// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"


// Called when the game starts or when spawned
void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

UTankAimingComponent* ATankAIController::GetControlledTankAimingComponent() const
{
	return Cast<UTankAimingComponent>(GetPawn()->GetComponentByClass(UTankAimingComponent::StaticClass()));
}

APawn* ATankAIController::GetPlayerTank() const
{
	auto PlayerTank = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerTank)
	{
		return PlayerTank;
	}
	else
	{
		return nullptr;
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	APawn* TargetPlayerTank = GetPlayerTank();
	if (TargetPlayerTank)
	{
		EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor( TargetPlayerTank, AcceptanceRadius );
		//UE_LOG(LogTemp, Warning, TEXT("T`%s, EPathFollowingRequestResult: %d"), *GetName(), MoveToActorResult);

		UTankAimingComponent* ControlledTankAimingComponent = GetControlledTankAimingComponent();
		ControlledTankAimingComponent->AimAt( TargetPlayerTank->GetActorLocation() );

		EFiringState FiringState = ControlledTankAimingComponent->GetFiringState();
		if (FiringState == EFiringState::Aiming)
			ControlledTankAimingComponent->Fire();
	}
}
