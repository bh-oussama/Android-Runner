// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "ShieldBase.generated.h"

/**
 * 
 */
UCLASS()
class ANDROIDRUNNER_API AShieldBase : public APickableBase
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere, Category = "Basic")
		int32 Armor = 100;

	UPROPERTY(EditAnywhere, Category = "Basic")
		int32 Duration = 5.f;

	virtual void PawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	
};
