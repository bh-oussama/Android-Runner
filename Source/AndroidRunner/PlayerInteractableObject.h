#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInteractableObject.generated.h"

UCLASS()
class ANDROIDRUNNER_API APlayerInteractableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerInteractableObject();

	// objects that can be spawned after spawning this object
	UPROPERTY(EditAnywhere, Category = "Spawning logic")
		TArray<TSubclassOf<APlayerInteractableObject>> nextObjects;

	// objects that shouldn't be spawned after spawning this object
	UPROPERTY(EditAnywhere, Category = "Spawning logic")
		TArray<TSubclassOf<APlayerInteractableObject>> toNotSpawnNext;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* Root = nullptr;

	/**
	* At this component's location's X component the new obstacle/EnvStatic will be spawned
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* NextSpawnX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* NextTileSpawnOffset = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* Collision = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName PickableSplineTag = "PickableTrajectory";

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName SpecialPickableTag = "SpecialPickable";

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName  DecorationLocationTag = "Decoration";

	/* should be multiple of 2000 so tiles and objects are spawned correctly */
	UPROPERTY(EditAnywhere, Category = "Convention")
		int32 XLength = 2000;

	/* 
	*custom tile to spawn for this object
	*if null then the standard current mode is spawned
	*/
	UPROPERTY(EditAnywhere, Category = "Convention")
		TSubclassOf<class ATileBase> customTileClass = nullptr;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<class USplineComponent*> GetSplines() { return Splines; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<class USceneComponent*> GetPickablesLocation() { return pickableLocations; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetStaticMeshesCollision() { check(0 && "You must override SetStaticMeshesCollision"); };

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	TArray<class UStaticMeshComponent*> StaticMeshs;
	TArray<class USplineComponent*> Splines;
	TArray<class USceneComponent*> decorations;
	TArray<class USceneComponent*> pickableLocations;

	class AEndlessGM* GM = nullptr;

public:
	virtual void Tick(float DeltaTime) override;
};
