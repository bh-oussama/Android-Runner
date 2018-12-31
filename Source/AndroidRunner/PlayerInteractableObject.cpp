#include "PlayerInteractableObject.h"
#include "AndroidRunner.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyLib.h"
#include "EndlessGM.h"
#include "PickableSpawner.h"

// Sets default values
APlayerInteractableObject::APlayerInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Collision = CreateDefaultSubobject <UBoxComponent>("Collision");
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Collision->SetRelativeLocation(FVector(4000.f, 0.f, 0.f));
	Collision->SetRelativeScale3D(FVector(8.f, 17.f, 17.f));

}

// Called when the game starts or when spawned
void APlayerInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &APlayerInteractableObject::OnBeginOverlap);
	
	// getting meshes
	auto meshes = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (auto actor : meshes) {
		auto var = Cast<UStaticMeshComponent>(actor);
		if (var == NULL) return;

		var->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		var->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		var->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

		StaticMeshs.Add(var);
	}

	SetStaticMeshesCollision();

	// getting special pickable points
	auto picks = GetComponentsByTag(USceneComponent::StaticClass(), SpecialPickableTag);
	for (auto pick : picks) {
		pickableLocations.Add(Cast<USceneComponent>(pick));
	}

	// getting decoration points
	auto decs = GetComponentsByTag(USceneComponent::StaticClass(), DecorationLocationTag);
	for (auto dec : decs) {
		decorations.Add(Cast<USceneComponent>(dec));
	}

	// getting splines
	auto splines = GetComponentsByTag(USplineComponent::StaticClass(), PickableSplineTag);
	for (auto spline : splines) {
		Splines.Add(Cast<USplineComponent>(spline));
	}

	// TODO: adding splines for GM to reference
	GM = MyLib::GetEndlessGM(GetWorld());

	// TODO: adding special pickable locations for GM to reference

}

// Called every frame
void APlayerInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerInteractableObject::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(SpawningLog, Warning, TEXT("%s being destroyed"), *GetName());
	Destroy();
}
