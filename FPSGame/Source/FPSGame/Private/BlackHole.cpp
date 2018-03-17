// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComponent->SetSphereRadius(100);
	InnerSphereComponent->SetupAttachment(MeshComponent);
	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComponent->SetSphereRadius(3000);
	OuterSphereComponent->SetupAttachment(MeshComponent);
}

void ABlackHole::Pull()
{
	TArray<AActor*> boxes;
	OuterSphereComponent->GetOverlappingActors(OUT boxes);

	for (AActor* box : boxes)
	{
		UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(box->GetRootComponent());

		if (mesh)
			mesh->AddRadialForce(GetOwner()->GetActorLocation(), OuterSphereComponent->GetScaledSphereRadius(), -1000000, ERadialImpulseFalloff::RIF_Linear);	
	}
}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, OtherActor->GetOwner()->GetActorLocation());
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Pull();
}

