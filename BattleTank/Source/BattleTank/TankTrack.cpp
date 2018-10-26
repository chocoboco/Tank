// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"


UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

//void UTankTrack::BeginPlay()
//{
//	Super::BeginPlay();
//
//	//OnComponentHit.AddDynamic( this, &UTankTrack::OnHit );
//}

TArray<ASprungWheel*> UTankTrack::GetWheels()
{
	if (Wheels.Num() == 0)
	{
		TArray<USceneComponent*> Children;
		GetChildrenComponents(true, Children);

		for (USceneComponent* Child : Children)
		{
			auto SpawnPointChild = Cast<USpawnPoint>(Child);
			if (SpawnPointChild)
			{
				AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
				ASprungWheel* SprungWheel = Cast<ASprungWheel>(SpawnedChild);
				if (SprungWheel)
				{
					Wheels.Add( SprungWheel );
				}
			}
		}
	}

	return Wheels;
}

void UTankTrack::SetThrottle( float Throttle )
{
	float CurrentThrottle = FMath::Clamp<float>( Throttle, -1.0f, 1.0f );
	DrivingTrack( CurrentThrottle );
}

void UTankTrack::DrivingTrack( float CurrentThrottle )
{
	float ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	TArray<ASprungWheel*> Wheels = GetWheels();
	float ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce( ForcePerWheel );
	}

	//FVector ForceLocation = GetComponentLocation();
	//UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	//TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

	//UE_LOG(LogTemp, Warning, TEXT("%s | %f"), *GetName(), Throttle);
}

//// 달아놓은 component 의 simulated physics 가 켜져있느냐 아니냐에 따라 계속 호출되느냐, 한번만 호출되느냐 달라짐.
//void UTankTrack::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	ApplySidewaysForce();
//	CurrentThrottle = 0.0f;
//
//	//UE_LOG(LogTemp, Warning, TEXT("Tank Track OnHit(): %s"), *GetName());
//}

//void UTankTrack::ApplySidewaysForce()
//{
//	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
//	if (0.0f != SlippageSpeed)
//	{
//		// Work-out the required acceleration this frame to correct
//		float DeltaTime = GetWorld()->GetDeltaSeconds();
//		FVector CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * GetRightVector();
//
//		// Calculate and apply sideways (F = m a)
//		UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
//		FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2.0f; // 2 tracks
//		TankRoot->AddForce(CorrectionForce);
//	}
//}
