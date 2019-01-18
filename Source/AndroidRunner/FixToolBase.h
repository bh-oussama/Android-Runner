// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "FixToolBase.generated.h"

/**
 *
 */
UCLASS()
class ANDROIDRUNNER_API AFixToolBase : public APickableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float FixAmount = 35.f;

	virtual void PawnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

};
