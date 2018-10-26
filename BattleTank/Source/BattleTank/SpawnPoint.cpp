// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USpawnPoint::USpawnPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>( SpawnClass, GetComponentTransform() );
	//AActor* NewActor = GetWorld()->SpawnActor<AActor>(SpawnClass);	// 즉시 Spawn 된 Actor 의 BeginPlay() 까지 호출됨.
	if (SpawnedActor)
	{
		SpawnedActor->AttachToComponent( this, FAttachmentTransformRules::KeepWorldTransform );
		UGameplayStatics::FinishSpawningActor( SpawnedActor, GetComponentTransform() );
	}
}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor * USpawnPoint::GetSpawnedActor() const
{
	return SpawnedActor;
}

