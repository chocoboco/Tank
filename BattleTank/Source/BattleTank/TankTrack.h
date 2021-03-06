// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

class ASprungWheel;

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent), hideCategories = ("Custom"))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle( float Throttle );
	void DrivingTrack( float CurrentThrottle );

	//virtual void BeginPlay() override;

	TArray<ASprungWheel*> GetWheels();

	//void ApplySidewaysForce();
	
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 4000000.0f;

protected:
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UTankTrack();

	TArray<ASprungWheel*> Wheels;
};
