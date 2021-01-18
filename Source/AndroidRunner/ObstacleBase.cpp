#include "ObstacleBase.h"
#include "Engine/World.h"
#include "EndlessGM.h"
#include "Kismet/GameplayStatics.h"
#include "PickableSpawner.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyLib.h"

AObstacleBase::AObstacleBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();
}

void AObstacleBase::SetStaticMeshesCollision()
{
	for (auto mesh : StaticMeshs)
	{
		mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	}
}

void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GoesUpAndDown)
	{
		if (goingUp)
		{
			for (auto mesh : StaticMeshs)
			{
				mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y, mesh->GetRelativeLocation().Z + VerticalMovementSpeed * DeltaTime));
				if (mesh->GetRelativeLocation().Z >= MaxZ)
				{
					goingUp = false;
					mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y, MaxZ));
				}
			}
		}
		else {
			if (Falls)
			{
				for (auto mesh : StaticMeshs)
				{
					fallingSpeed += GravityScale * 981.f * DeltaTime;
					mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y, mesh->GetRelativeLocation().Z - fallingSpeed));
					if (mesh->GetRelativeLocation().Z <= MinZ)
					{
						goingUp = true;
						fallingSpeed = 0.f;
						mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y, MinZ));
					}
				}
			}
			else {
				for (auto mesh : StaticMeshs)
				{
					mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y, mesh->GetRelativeLocation().Z - VerticalMovementSpeed * DeltaTime));
					if (mesh->GetRelativeLocation().Z <= MinZ)
					{
						goingUp = true;
						mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y, MinZ));
					}
				}
			}
		}
	}

	if (GoesRightAndLeft) {
		if (goingLeft) {
			for (auto mesh : StaticMeshs)
			{
				mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y - HorizontalMovementSpeed * DeltaTime, mesh->GetRelativeLocation().Z));
				if (mesh->GetRelativeLocation().Y <= MinY)
				{
					goingLeft = false;
					mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, MinY, mesh->GetRelativeLocation().Z));
				}
			}
		}
		else {
			for (auto mesh : StaticMeshs)
			{
				mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, mesh->GetRelativeLocation().Y + HorizontalMovementSpeed * DeltaTime, mesh->GetRelativeLocation().Z));
				if (mesh->GetRelativeLocation().Y >= MaxY)
				{
					goingLeft = true;
					mesh->SetRelativeLocation(FVector(mesh->GetRelativeLocation().X, MaxY, mesh->GetRelativeLocation().Z));
				}
			}
		}
	}

	if (DoesRotate)
	{
		for (auto mesh : StaticMeshs)
		{
			mesh->AddLocalRotation(RotationSpeed*DeltaTime);
		}
	}
}

