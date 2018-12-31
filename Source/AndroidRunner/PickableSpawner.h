// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableSpawner.generated.h"

UCLASS()
class ANDROIDRUNNER_API APickableSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickableSpawner();

	// List of pickables which this spawner will be able to spawn.
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class APickableBase>> AvailablePickables;

	UPROPERTY(EditAnywhere)
		float DistanceBetweenCoins = 300.f;

	/*	Spawns a given pickable in a given location.
	*	If pickable's class was not specified it will spawn a random pickable from available pickables.
	*/
	UFUNCTION(BlueprintCallable)
		void SpawnPickable(FVector Location, TSubclassOf<class APickableBase> PickableClass = nullptr);

	UFUNCTION(BlueprintCallable)
		void SpawnAlongSpline(USplineComponent* spline);

	UFUNCTION(BlueprintCallable)
		void SetCoinClass(TSubclassOf<class APickableBase> coinClass) { this->currCoinClass = coinClass; }

	TArray<class USplineComponent*> splines;
	TArray<class USceneComponent*> specialPickables;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSubclassOf<class APickableBase> currCoinClass = nullptr;

public:
	virtual void Tick(float DeltaTime) override;

	// when this is equal 0, a magnet will be spawned
	int32 MagnetSpawning = 0;
};
