// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInteractableObject.h"
#include "EnvStaticBase.generated.h"

UCLASS()
class ANDROIDRUNNER_API AEnvStaticBase : public APlayerInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnvStaticBase();

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName DamagingCollidersTag = "OffLimit";

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetStaticMeshesCollision() override;

	TArray<class UBoxComponent*> OffLimits;

};
