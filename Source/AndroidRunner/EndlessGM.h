#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessGM.generated.h"

#define INTIAL_NUMBER_OF_TILES 15
#define RANGE_OF_VIEW 10000.f
#define OBJECT_SAFE_DISTANCE_TO_PLAYER_TO_DESTROY 9000.f

UENUM(BlueprintType)
enum Mode {
	Static = 0		UMETA(DisplayName = "Static"),
	Chinese = 1		UMETA(DisplayName = "Chinese"),
	Scary = 2		UMETA(DisplayName = "Scray"),
};

// TODO:	make the shield work

// TODO:	add sound to the game

// TODO:	clean code to simulate in editor with no ERROR

// TODO:	Add mode manager (changes the used tile class for this class)
//				*tile class can be changed between modes

/**
 * EndlessGM is the base game mode for any game mode that supports tiles and endless spawning of tiles, envStatics, obstacles and pickables.
 */
UCLASS()
class ANDROIDRUNNER_API AEndlessGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEndlessGM();

	bool bIsMagnetActivated = false;
	int32 CollectedCoinsCount = 0;
	float SpawnX = 4000.f;
	float MagnetRange = 0.f;
	float MagnetDeactivationTime = 0.f;
	float MagnetPower = 0.f;
	class USoundCue* MagnetSound = nullptr;
	class UAudioComponent* AudioSource = nullptr;

	UPROPERTY(EditAnywhere, Category = "General | Spawning")
		float DistanceBetweenCoins = 300.f;

	UFUNCTION(BlueprintCallable)
		virtual ATileBase* SpawnTile(TSubclassOf<ATileBase> tileClass = nullptr);


		/* ------------------- STATIC MODE CONFIG ------------------- */

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		float StaticModeEndLocationX = 80000.f;

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		float StaticModeDuration = 30.f;

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		class USoundBase* StaticModeBackgroundMusic = nullptr;

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		TArray<TSubclassOf<class AEnvStaticBase>> StaticModeEnvStatics;

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		TArray<TSubclassOf<class AObstacleBase>> StaticModeObstacles;

	/*
	*1st: coin class
	*/
	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		TArray<TSubclassOf<class APickableBase>> StaticModePickables;

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		TSubclassOf<class ATileBase> StaticModeTileClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Modes | Static")
		TArray< TSubclassOf<class ATileBase>> StaticModeTransitionTiles;


		/* ------------------- CHINESE MODE CONFIG ------------------- */
	
	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		float ChineseModeDuration = 30.f;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		float ChineseModeEndLocationX = 300000.f;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		class USoundBase* ChineseModeBackgroundMusic = nullptr;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		TArray<TSubclassOf<class AEnvStaticBase>> ChineseModeEnvStatics;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		TArray<TSubclassOf<class AObstacleBase>> ChineseModeObstacles;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		TArray<TSubclassOf<class APickableBase>> ChineseModePickables;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		TSubclassOf<class ATileBase> ChineseModeTileClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Modes | chinese")
		TArray< TSubclassOf<class ATileBase>> ChineseModeTransitionTiles;


		/* -------------------	SCARY MODE CONFIG ------------------- */

	UPROPERTY(EditAnywhere, Category = "Modes | Scary")
		TArray<TSubclassOf<class AEnvStaticBase>> ScaryModeEnvStatics;

	UPROPERTY(EditAnywhere, Category = "Modes | Scary")
		TArray<TSubclassOf<class AObstacleBase>> ScaryModeObstacles;

	UPROPERTY(EditAnywhere, Category = "Modes | Scary")
		TArray<TSubclassOf<class APickableBase>> ScaryModePickables;

	UPROPERTY(EditAnywhere, Category = "Modes | Scary")
		TSubclassOf<class ATileBase> ScaryTileClass = nullptr;

	// spawns a tile and all complementary objects (Complementary objects like pickables, and envStatics or obstacles)
	UFUNCTION(BlueprintCallable)
		void SpawnAddition();

	UFUNCTION(BlueprintCallable)
		void SpawnCoinsAlongSpline(USplineComponent* spline);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TSubclassOf<class APlayerInteractableObject> NextSpawnable();

	UFUNCTION(BlueprintCallable)
		void SwitchToNextMode();

	UFUNCTION(BlueprintCallable)
		void SpawnTransitionTiles(TArray<TSubclassOf<ATileBase>> tiles);

	/**
	* returns a random pickable, coins aren't included because they exist in index 0 of pickables array
	*/
	UFUNCTION(BlueprintCallable)
		TSubclassOf<APickableBase> GetRandomSpecialPickable();

	UFUNCTION(BlueprintCallable)
		void UpdatePickablesSpawningTime();

protected:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	class APawnBase* Pawn = nullptr;
	class APickableSpawner* pickableSpawner = nullptr;
	FTransform NewSpawnTransform = FTransform();

	// holds EnvStatics and Obstacles to spawn when the player reaches their location
	TArray<TSubclassOf<class APlayerInteractableObject>> objectsToSpawn;
	TSubclassOf<class APickableBase> currCoinClass = nullptr;

	// holds special pickables' locations
	TArray<FVector> specialPickableLocations;
	float PickablesSpawningTime = 0.f;

	// holds all actors added and destroys the ones which the player has passed
	TArray<AActor*> spawnedObjects;
	float currModeEndInTime = 0.f;
	float currModeDuration = 0.f;
	float currModeEndX = 0.f;
	TEnumAsByte	<Mode> currMode = Mode::Static;
	class USoundBase* currBackgroundMusic = nullptr;
	TSubclassOf<class ATileBase> currTileClass = nullptr;


};
