// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine//PhysicsConstraintComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	BodyWheelPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Body Wheel Physics Constraint"));
	SetRootComponent(BodyWheelPhysicsConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	//Axle->AttachToComponent(BodyWheelPhysicsConstraint, FAttachmentTransformRules::KeepRelativeTransform);
	Axle->SetupAttachment(BodyWheelPhysicsConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	//Wheel->AttachToComponent(Axle, FAttachmentTransformRules::KeepRelativeTransform);
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Axle Wheel Physics Constraint"));
	//AxleWheelConstraint->AttachToComponent(Axle, FAttachmentTransformRules::KeepRelativeTransform);
	AxleWheelConstraint->SetupAttachment(Axle);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision( true );
	Wheel->OnComponentHit.AddDynamic( this, &ASprungWheel::OnHit );

	SetupConstraint();
}

void ASprungWheel::SetupConstraint()
{
	AActor* ParentActor = GetAttachParentActor();
	ensure(ParentActor);
	if (ParentActor == nullptr) { return; }

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	ensure(BodyRoot);
	if (BodyRoot == nullptr) { return; }

	BodyWheelPhysicsConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents( Axle, NAME_None, Wheel, NAME_None );
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}

// 땅에 닿았을 때만 이동 시켜주기 위함.
// TG_PostPhysics 으로 설정했기 때문에 OnHit() 가 Physics 처리 후 먼저 호출되고 Tick() 이 호출됨.
void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if( 0.0f != TotalForceMagnitudeThisFrame )
	{
		ApplyForce();
	}

	//UE_LOG(LogTemp, Warning, TEXT( "ASprungWheel::OnHit(): %s | CurrentForceMagnitude: %f" ), *GetName(), TotalForceMagnitudeThisFrame);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0.0f;

		//UE_LOG(LogTemp, Warning, TEXT("ASprungWheel::Tick(): %s | TickGroup: %i"), *GetName(), GetWorld()->TickGroup);
	}
}


void ASprungWheel::AddDrivingForce( float ForceMagnitude )
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}