// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInteractableObject.h"
#include "ObstacleBase.generated.h"

// TODO: remove spline component and make it an array filled at BeginPlay

UCLASS()
class ANDROIDRUNNER_API AObstacleBase : public APlayerInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObstacleBase();

	UPROPERTY(BlueprintReadWrite)
		float SafeDistance = 100.f;

	// Check this if your obstacle moves up and down.
	UPROPERTY(EditAnywhere)
		bool GoesUpAndDown = false;

	// The maximum height the obstacle can reach (this has effect only if you have checked GoesUpAndDown)
	UPROPERTY(EditAnywhere)
		float MaxZ = 0.f;

	// The minimum height the obstacle can reach (this has effect only if you have checked GoesUpAndDown)
	UPROPERTY(EditAnywhere)
		float MinZ = 0.f;

	// Movement speed.
	UPROPERTY(EditAnywhere)
		float VerticalMovementSpeed = 0.f;

	// Check if you want the obstacle to simulate falling when going down.
	UPROPERTY(EditAnywhere)
		bool Falls = false;

	// Gravity scale (multiplier) for falling speed.
	UPROPERTY(EditAnywhere)
		float GravityScale = 1.f;

	// Check this if your obstacle moves up and down.
	UPROPERTY(EditAnywhere)
		bool GoesRightAndLeft = false;

	// Right limit that this obstacle can reach (this has effect only if you have checked GoesRightAndLeft)
	UPROPERTY(EditAnywhere)
		float MaxY = 0.f;

	// Left limit that this obstacle can reach (this has effect only if you have checked GoesRightAndLeft)
	UPROPERTY(EditAnywhere)
		float MinY = 0.f;

	// Movement speed.
	UPROPERTY(EditAnywhere)
		float HorizontalMovementSpeed = 0.f;

	// Check if this obstacle rotates
	UPROPERTY(EditAnywhere)
		bool DoesRotate = false;

	/*
	*	Each component (Roll, Pitch or Yaw) represents the rotation speed arround the corresponding euler axis.
	*	The greater the value the more pace it will rotate faster.
	*/
	UPROPERTY(EditAnywhere)
		FRotator RotationSpeed;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetStaticMeshesCollision() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool goingUp = true;
	bool goingLeft = false;
	float fallingSpeed = 0.f;
};
