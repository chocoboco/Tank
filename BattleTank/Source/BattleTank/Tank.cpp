// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"



// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;
}

float ATank::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt( Damage );
	int32 DamageToApply = FMath::Clamp( DamagePoints, 0, CurrentHealth );
	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		OnActorDead.Broadcast();
	}

	UE_LOG( LogTemp, Warning, TEXT("[TakeDamage() from %s] DamageToApply: %d | CurrentHealth: %d"), *GetName(), DamageToApply, CurrentHealth );

	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return (static_cast<float>(CurrentHealth) / static_cast<float>(StartingHealth));
}

//// Called to bind functionality to input
//void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}