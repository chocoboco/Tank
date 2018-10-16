// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::Initialize(UTankTrack * LeftTrackToSet, UTankTrack * RightTrackToSet)
{
	if (LeftTrackToSet && RightTrackToSet)
	{
		LeftTrack = LeftTrackToSet;
		RightTrack = RightTrackToSet;
	}
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	//UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);

	if( LeftTrack && RightTrack )
	{
		LeftTrack->SetThrottle( Throw );
		RightTrack->SetThrottle( Throw );
	}
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	//UE_LOG(LogTemp, Warning, TEXT("Intend turn right throw: %f"), Throw);
	
	if (LeftTrack && RightTrack)
	{
		LeftTrack->SetThrottle( Throw );
		RightTrack->SetThrottle( -Throw );
	}
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	//Super::RequestDirectMove( MoveVelocity, bForceMaxSpeed );

	FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIForwardIntention = MoveVelocity.GetSafeNormal();
	float ForwardThrow = FVector::DotProduct( TankForward, AIForwardIntention );
	IntendMoveForward( ForwardThrow );

	float RightThrow = FVector::CrossProduct( TankForward, AIForwardIntention ).Z;
	IntendTurnRight( RightThrow );

	//UE_LOG(LogTemp, Warning, TEXT("RequestDirectMove ForwardThrow: %f"), ForwardThrow);

}
