// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "CoinBase.generated.h"

/**
 * 
 */
UCLASS()
class ANDROIDRUNNER_API ACoinBase : public APickableBase
{
	GENERATED_BODY()

public:
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
private:
	class AEndlessGM* gm = nullptr;
	class APawn* player = nullptr;
	
};
