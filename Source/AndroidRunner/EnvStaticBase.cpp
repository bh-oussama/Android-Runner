#include "EnvStaticBase.h"
#include "AndroidRunner.h"
#include "Kismet/GameplayStatics.h"
#include "PickableSpawner.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AEnvStaticBase::AEnvStaticBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnvStaticBase::BeginPlay()
{
	Super::BeginPlay();

	// getting  off limit collision
	auto offLimits = GetComponentsByTag(UBoxComponent::StaticClass(), DamagingCollidersTag);
	UE_LOG(SpawningLog, Warning, TEXT("off limits count: %d"), OffLimits.Num());
	for (auto actor : offLimits) {
		auto var = Cast<UBoxComponent>(actor);
		if (var == NULL) return;
		if (var->ComponentHasTag(DamagingCollidersTag)) {
			var->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			var->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
			var->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			var->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

			OffLimits.Add(var);
		}
	}
}

void AEnvStaticBase::SetStaticMeshesCollision()
{
	for (auto mesh : StaticMeshs) {
		mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	}
}
