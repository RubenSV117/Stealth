// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"


// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComponent->SetSphereRadius(100);
    InnerSphereComponent->SetupAttachment(MeshComp);
	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComponent->SetSphereRadius(3000);
	OuterSphereComponent->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
		OtherActor->Destroy();
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Find all overlapping components that are simulating physics and pull them in
	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterSphereComponent->GetOverlappingComponents(OUT OverlappingComps);

	for (size_t i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* overlappedComp = OverlappingComps[i];

		if (overlappedComp && overlappedComp->IsSimulatingPhysics())
		{
			const float sphereRadius = OuterSphereComponent->GetScaledSphereRadius();
			const float pullStrength = -2000;

			overlappedComp->AddRadialForce(GetActorLocation(), sphereRadius, pullStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}


}

