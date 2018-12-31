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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* Collision = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName PickableSplineTag = "PickableTrajectory";

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName SpecialPickableTag = "SpecialPickable";

	UPROPERTY(VisibleAnywhere, Category = "Convention")
		FName  DecorationLocationTag = "Decoration";

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<class USplineComponent*> GetSplines() { return Splines; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<class USceneComponent*> GetPickablesLocation() { return pickableLocations; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetStaticMeshesCollision() { check(0 && "You must override this"); };

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	TArray<class UStaticMeshComponent*> StaticMeshs;
	TArray<class USplineComponent*> Splines;
	TArray<class USceneComponent*> decorations;
	TArray<class USceneComponent*> pickableLocations;

	class AEndlessGM* GM = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};
