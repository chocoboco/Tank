// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent( CollisionMesh );
	CollisionMesh->SetNotifyRigidBodyCollision( true );	// Collision -> Simulation Generates Hit Events
	CollisionMesh->SetVisibility( false );

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	//OnActorHit.AddDynamic( this, &AProjectile::OnHit );
	CollisionMesh->OnComponentHit.AddDynamic( this, &AProjectile::OnHit );
}

// 달아놓은 component 의 simulated physics 가 켜져있느냐 아니냐에 따라 계속 호출되느냐, 한번만 호출되느냐 달라짐.
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();

	UE_LOG(LogTemp, Warning, TEXT("Projectile Landed: %s"), *GetName());
}

//// Called every frame
//void AProjectile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AProjectile::LaunchProjectile(float Speed)
{
	float Time = GetWorld()->GetTimeSeconds();
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

