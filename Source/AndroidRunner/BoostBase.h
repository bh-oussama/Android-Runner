// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "BoostBase.generated.h"

/**
 *
 */
UCLASS()
class ANDROIDRUNNER_API ABoostBase : public APickableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Duration = 5.f;

	UPROPERTY(EditAnywhere)
		float Acceleration = 500.f;

	UPROPERTY(EditAnywhere)
		float AddedSpeedLimit = 300.f;

	virtual void PawnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

};
