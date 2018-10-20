// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	OnComponentHit.AddDynamic( this, &UTankTrack::OnHit );
}

void UTankTrack::SetThrottle( float Throttle )
{
	CurrentThrottle = FMath::Clamp<float>( CurrentThrottle + Throttle, -1.0f, 1.0f );
}

void UTankTrack::DrivingTrack()
{
	//FMath::Max()
	FVector ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

	//UE_LOG(LogTemp, Warning, TEXT("%s | %f"), *GetName(), Throttle);
}

// 달아놓은 component 의 simulated physics 가 켜져있느냐 아니냐에 따라 계속 호출되느냐, 한번만 호출되느냐 달라짐.
void UTankTrack::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrivingTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.0f;

	//UE_LOG(LogTemp, Warning, TEXT("Tank Track OnHit(): %s"), *GetName());
}

void UTankTrack::ApplySidewaysForce()
{
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	if (0.0f != SlippageSpeed)
	{
		// Work-out the required acceleration this frame to correct
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		FVector CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * GetRightVector();

		// Calculate and apply sideways (F = m a)
		UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
		FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2.0f; // 2 tracks
		TankRoot->AddForce(CorrectionForce);
	}
}
