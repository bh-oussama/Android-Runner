// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "MagnetBase.generated.h"

/**
 * 
 */
UCLASS()
class ANDROIDRUNNER_API AMagnetBase : public APickableBase
{
	GENERATED_BODY()
	
public:
	// Range of magnet.
	UPROPERTY(EditAnywhere, Category = "Magnetic")
		float Range = 800.f;

	// Duration of magnet.
	UPROPERTY(EditAnywhere, Category = "Magnetic")
		float Duration = 3.f;

	// Power of magnet.
	UPROPERTY(EditAnywhere, Category = "Magnetic")
		float Power = 100.f;

	// Played while this magnet is activated.
	UPROPERTY(EditAnywhere, Category = "Magnetic")
		class USoundCue* MagneticSound = nullptr;

protected:
	virtual void PawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

private:
	
};
