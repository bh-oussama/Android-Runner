#include "EndlessGM.h"
#include "AndroidRunner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PlayerInteractableObject.h"
#include "PawnBase.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundConcurrency.h"
#include "ObstacleBase.h"
#include "EnvStaticBase.h"
#include "PickableBase.h"
#include "TileBase.h"
#include "Components/ArrowComponent.h"


AEndlessGM::AEndlessGM() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AEndlessGM::SpawnAddition()
{
	auto toSpawn = NextSpawnable();
	if (toSpawn) {
		FTransform transform;
		transform.SetLocation(FVector(SpawnX, 0.f, 0.f));
		auto spawned = GetWorld()->SpawnActor<APlayerInteractableObject>(toSpawn, transform);
		SpawnX = spawned->Collision->GetComponentLocation().X;
		for (auto obj : spawned->nextObjects) {
			objectsToSpawn.AddUnique(obj);
		}
		for (auto one : spawned->toNotSpawnNext) {
			objectsToSpawn.Remove(one);
		}
		for (auto spline : spawned->GetSplines()) {
			SpawnCoinsAlongSpline(spline);
		}
		for (auto sceneComp : spawned->GetPickablesLocation()) {
			specialPickableLocations.Add(sceneComp->GetComponentLocation());
		}
		if (PickablesSpawningTime < GetWorld()->GetTimeSeconds() && spawned->GetPickablesLocation().Num() > 0) {
			for (auto location : spawned->GetPickablesLocation()) {
				FTransform transform;
				transform.SetLocation(location->GetComponentLocation());
				auto pickable = GetWorld()->SpawnActor<APickableBase>(GetRandomSpecialPickable(), transform);
				spawnedObjects.Add(pickable);
				UE_LOG(SpawningLog, Warning, TEXT("spawned Pickable %s"), *pickable->GetName());
			}
			UpdatePickablesSpawningTime();
		}
		UE_LOG(SpawningLog, Warning, TEXT("spawned %s"), *spawned->GetName());
	}
	else {
		UE_LOG(SpawningLog, Warning, TEXT("no object to spawn"));
	}
}

void AEndlessGM::SpawnCoinsAlongSpline(USplineComponent* spline)
{
	check(currCoinClass);
	float distance = 0.f;
	while (distance <= spline->GetSplineLength()) {
		auto pickable = GetWorld()->SpawnActor<APickableBase>(
			currCoinClass,
			spline->GetTransformAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World));
		UE_LOG(SpawningLog, Warning, TEXT("spawned %s"), *GetName());
		distance += DistanceBetweenCoins;
		spawnedObjects.Add(pickable);
	}
}

TSubclassOf<APlayerInteractableObject> AEndlessGM::NextSpawnable()
{
	if (objectsToSpawn.Num() > 0) {
		return objectsToSpawn[UKismetMathLibrary::RandomInteger(objectsToSpawn.Num())];
	}
	else {
		return nullptr;
	}
}

void AEndlessGM::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	/*
		if (GetWorld()->GetTimeSeconds() > currModeEndInTime) {
			SwitchToNextMode();
		}
		*/
	if (Pawn->GetActorLocation().X > currModeEndX)
		SwitchToNextMode();

	// updating VARs relative to magnet
	if (bIsMagnetActivated) {
		if (AudioSource->IsActive() == false) {
			AudioSource->Activate();
		}
		if (MagnetDeactivationTime < GetWorld()->GetTimeSeconds()) {
			bIsMagnetActivated = false;
			AudioSource->Stop();
			AudioSource->Deactivate();
			AudioSource->DestroyComponent();
		}
	}

	// destroying passed actors
	auto playerX = Pawn->GetActorLocation().X;
	TArray<AActor*> objectsToDelete; // array holding object to delete
	objectsToDelete.Empty();
	if (spawnedObjects.Num() > 0) {
		for (auto obj : spawnedObjects) {
			if (obj->GetActorLocation().X < playerX - OBJECT_SAFE_DISTANCE_TO_PLAYER_TO_DESTROY) {
				objectsToDelete.Add(obj);
				obj->Destroy();
			}
		}
	}

	// removing destroyed objects passed by the player (and away with a safe distance) from the spawnedObjects's array
	for (auto obj : objectsToDelete)
		spawnedObjects.Remove(obj);

	// spawning EnvStatic and Obstacle
	if (playerX + RANGE_OF_VIEW > SpawnX) {
		SpawnAddition();
	}
}

void AEndlessGM::BeginPlay()
{
	Super::BeginPlay();

	Pawn = Cast<APawnBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	objectsToSpawn.Append(StaticModeEnvStatics);
	objectsToSpawn.Append(StaticModeObstacles);

	currTileClass = StaticModeTileClass;

	check(StaticModePickables.Num() > 0 && "no pickables array of any mode should be empty");
	currCoinClass = StaticModePickables[0];
	currModeDuration = StaticModeDuration;
	currModeEndInTime = StaticModeDuration;
	currModeEndX = StaticModeEndLocationX;
	UpdatePickablesSpawningTime();

	// Spawning initial tiles
	NewSpawnTransform.SetLocation(FVector(-500, 0, 0));
	for (int i = 0; i < INTIAL_NUMBER_OF_TILES; i++)
	{
		SpawnTile();
	}

	if (StaticModeBackgroundMusic)
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			StaticModeBackgroundMusic
		);
}

void AEndlessGM::SwitchToNextMode()
{
	UE_LOG(LogTemp, Warning, TEXT("switching to next mode"));
	currMode = (TEnumAsByte<Mode>) ((currMode + 1) % (Mode::Scary + 1));	// update Mode::scary to the latest mode in enum mode
	UE_LOG(LogTemp, Warning, TEXT("current mode: %d"), (int32)currMode);
	objectsToSpawn.Empty();
	switch (currMode)
	{
	case Mode::Static: {
		currModeDuration = StaticModeDuration;
		currModeEndInTime = StaticModeDuration + GetWorld()->GetTimeSeconds();
		objectsToSpawn.Append(StaticModeEnvStatics);
		objectsToSpawn.Append(StaticModeObstacles);
		currCoinClass = StaticModePickables[0];
		currBackgroundMusic = StaticModeBackgroundMusic;
		currTileClass = StaticModeTileClass;
		SpawnTransitionTiles(StaticModeTransitionTiles);
		currModeEndX += StaticModeEndLocationX;
		break;
	}
	case Mode::Chinese: {
		currModeDuration = ChineseModeDuration;
		currModeEndInTime = ChineseModeDuration + GetWorld()->GetTimeSeconds();
		objectsToSpawn.Append(ChineseModeEnvStatics);
		objectsToSpawn.Append(ChineseModeObstacles);
		currCoinClass = ChineseModePickables[0];
		currBackgroundMusic = ChineseModeBackgroundMusic;
		currTileClass = ChineseModeTileClass;
		SpawnTransitionTiles(ChineseModeTransitionTiles);
		currModeEndX += ChineseModeEndLocationX;
		break;
	}
	default:
		break;
	}

}

void AEndlessGM::SpawnTransitionTiles(TArray<TSubclassOf<ATileBase>> tiles)
{
	for (auto tileClass : tiles) {
		UE_LOG(LogTemp, Warning, TEXT("spawning transition tile"));
		SpawnX = SpawnTile(tileClass)->AttachementPoint->GetComponentLocation().X + 4000.f;
	}
}

TSubclassOf<APickableBase> AEndlessGM::GetRandomSpecialPickable()
{
	TArray<TSubclassOf<APickableBase>>* pickables = nullptr;
	switch (currMode)
	{
	case Mode::Static: {
		pickables = &StaticModePickables;
		break;
	}
	case Mode::Chinese: {
		pickables = &ChineseModePickables;
		break;
	}
	case Mode::Scary: {
		pickables = &ScaryModePickables;
		break;
	}
	default:
		UE_LOG(LogTemp, Error, TEXT("EndlessGM: current mode is unkonwn"));
		return nullptr;
	}
	if (pickables->Num()) {
		int index = UKismetMathLibrary::RandomInteger(pickables->Num());
		return (*pickables)[((index) ? index : 1)];
	}
	return nullptr;
}

void AEndlessGM::UpdatePickablesSpawningTime()
{
	PickablesSpawningTime = UKismetMathLibrary::RandomFloatInRange(3.f, 5.f);
}

ATileBase* AEndlessGM::SpawnTile(TSubclassOf<ATileBase> tileClass)
{
	if (tileClass) {
		FActorSpawnParameters param = FActorSpawnParameters();
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		UWorld* const World = GetWorld();
		check(World);

		auto Tile = GetWorld()->SpawnActor<ATileBase>(tileClass, NewSpawnTransform);
		check(Tile);
		NewSpawnTransform = Tile->AttachementPoint->GetComponentTransform();
		UE_LOG(SpawningLog, Warning, TEXT("spawned %s"), *Tile->GetName());
		return Tile;
	}
	else {
		check(currTileClass);
		FActorSpawnParameters param = FActorSpawnParameters();
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		UWorld* const World = GetWorld();
		check(World);

		auto Tile = GetWorld()->SpawnActor<ATileBase>(currTileClass, NewSpawnTransform);
		check(Tile);
		NewSpawnTransform = Tile->AttachementPoint->GetComponentTransform();

		UE_LOG(SpawningLog, Warning, TEXT("spawned %s"), *Tile->GetName());
		return Tile;
	}
}
