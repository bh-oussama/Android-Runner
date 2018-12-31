// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableBase.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

APickableBase::APickableBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickableBase::OnPawnOverlap);

}

float APickableBase::DP_LastminCurrent_LastminFirst()
{
	return FVector::DotProduct(LastLocation - Mesh->RelativeLocation, LastLocation - FirstLocation);
}

bool APickableBase::OutOfRange()
{
	FVector curr2last = LastLocation - Mesh->RelativeLocation;
	FVector first2last = LastLocation - FirstLocation;
	return FVector::DotProduct(curr2last, first2last) > 0 && (curr2last.Size() < first2last.Size());
}

void APickableBase::BeginPlay()
{
	Super::BeginPlay();
}

void APickableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DoesRotate) {
		Mesh->AddLocalRotation(RotationSpeed*DeltaTime);
	}
	if (DoesMove) {
		if (bFirst2Last) {
			Mesh->SetRelativeLocation(Mesh->RelativeLocation + MovementSpeed * DeltaTime);
			if (OutOfRange()) {
				Mesh->SetRelativeLocation(LastLocation);
			}
		}
		else {
			Mesh->SetRelativeLocation(Mesh->RelativeLocation - MovementSpeed * DeltaTime);
			if (OutOfRange()) {
				Mesh->SetRelativeLocation(FirstLocation);
			}
		}
	}
}

void APickableBase::OnPawnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (PickSound != nullptr)
		UGameplayStatics::PlaySound2D(GetWorld(), PickSound);
	PawnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
