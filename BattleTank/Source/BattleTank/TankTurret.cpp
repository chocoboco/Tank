// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"


void UTankTurret::Rotate( float RelativeSpeed )
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, 1.0f);
	float RotationChange = RelativeSpeed * MaxDegreePerSeconds * GetWorld()->DeltaTimeSeconds;
	float RawNewRotation = RelativeRotation.Yaw + RotationChange;
	//float Rotation = FMath::Clamp<float>(RawNewRotation,);

	SetRelativeRotation( FRotator(0.0f, RawNewRotation, 0.0f) );
}


