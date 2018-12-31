// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableBase.generated.h"

/**
 *
 */
UCLASS()
class ANDROIDRUNNER_API APickableBase : public AActor
{
	GENERATED_BODY()

public:
	APickableBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh = nullptr;

	/* Effect */

	// played when this pickable is picked by the player.
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		class USoundCue* PickSound = nullptr;

	/* Movement */

	// check if the mesh will rotate.
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		bool DoesRotate = false;
	// Rotation Speed which the mesh will rotate with (has effect only when DoesRotate is checked).
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		FRotator RotationSpeed = FRotator(0);

	// check if the mesh moves.
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		bool DoesMove = false;

	/*
	*	Movement speed which the mesh moves with (has effect only when DoesMove is checked).
	*	Mesh will move between FirstLocation & LastLocation.
	*/
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		FVector MovementSpeed = FVector(0);

	// the mesh will move between this location and the last location
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		FVector FirstLocation = FVector(0);
	// the mesh will move between this location and the first location
	UPROPERTY(EditAnywhere, Category = "Behaviour")
		FVector LastLocation = FVector(0);

	// what happens when the pawn overlaps with this pickable object.
	UFUNCTION(BlueprintCallable)
		virtual void PawnOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult & SweepResult){}

	/* returns dot product of vectors:
	 *	LastLocation - CurrentLocation	(of the mesh)
	 *	LastLocation - FirstLocation
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float DP_LastminCurrent_LastminFirst();

	/*
	*	returns true if Mesh is in-between FirstLocation and LastLocation,
	*	returns false if not.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool OutOfRange();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	bool bFirst2Last = true;
};
