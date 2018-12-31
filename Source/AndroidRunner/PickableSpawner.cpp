// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableSpawner.h"
#include "MyLib.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "PickableBase.h"
#include "Components/SplineComponent.h"


// Sets default values
APickableSpawner::APickableSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;
}

void APickableSpawner::SpawnPickable(FVector Location, TSubclassOf<APickableBase> PickableClass)
{
	if (PickableClass == nullptr) {
		PickableClass = AvailablePickables[UKismetMathLibrary::RandomInteger(AvailablePickables.Num())];
	}
	GetWorld()->SpawnActor<APickableBase>(PickableClass, FTransform(Location));
}

void APickableSpawner::SpawnAlongSpline(USplineComponent * spline)
{
	check(currCoinClass);
	float distance = 0.f;
	while (distance <= spline->GetSplineLength()) {
		auto pickable = GetWorld()->SpawnActor<APickableBase>(
			currCoinClass, 
			spline->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World));
		distance += DistanceBetweenCoins;
	}
}

// Called when the game starts or when spawned
void APickableSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickableSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MagnetSpawning = (MagnetSpawning > 0) ? MagnetSpawning - 1 : 0;
	if (splines.Num() > 0) {
		for (auto spline : splines) {
			MagnetSpawning = (MagnetSpawning > 0) ? MagnetSpawning - 1 : 0;
			float distance = 0.f;
			while (distance < spline->GetSplineLength()) {
				if (MagnetSpawning == 0) {
					if (distance >= 300.f) {
						SpawnPickable(FVector(spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World)), AvailablePickables[1]);
						distance += DistanceBetweenCoins;
						MagnetSpawning = UKismetMathLibrary::RandomIntegerInRange(7, 18);
					}
				}
				SpawnPickable(FVector(spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World)), AvailablePickables[0]);
				distance += DistanceBetweenCoins;
			}
		}
		splines.Empty();
	}
}

