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
				mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y, mesh->RelativeLocation.Z + VerticalMovementSpeed * DeltaTime));
				if (mesh->RelativeLocation.Z >= MaxZ)
				{
					goingUp = false;
					mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y, MaxZ));
				}
			}
		}
		else {
			if (Falls)
			{
				for (auto mesh : StaticMeshs)
				{
					fallingSpeed += GravityScale * 981.f * DeltaTime;
					mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y, mesh->RelativeLocation.Z - fallingSpeed));
					if (mesh->RelativeLocation.Z <= MinZ)
					{
						goingUp = true;
						fallingSpeed = 0.f;
						mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y, MinZ));
					}
				}
			}
			else {
				for (auto mesh : StaticMeshs)
				{
					mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y, mesh->RelativeLocation.Z - VerticalMovementSpeed * DeltaTime));
					if (mesh->RelativeLocation.Z <= MinZ)
					{
						goingUp = true;
						mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y, MinZ));
					}
				}
			}
		}
	}

	if (GoesRightAndLeft) {
		if (goingLeft) {
			for (auto mesh : StaticMeshs)
			{
				mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y - HorizontalMovementSpeed * DeltaTime, mesh->RelativeLocation.Z));
				if (mesh->RelativeLocation.Y <= MinY)
				{
					goingLeft = false;
					mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, MinY, mesh->RelativeLocation.Z));
				}
			}
		}
		else {
			for (auto mesh : StaticMeshs)
			{
				mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, mesh->RelativeLocation.Y + HorizontalMovementSpeed * DeltaTime, mesh->RelativeLocation.Z));
				if (mesh->RelativeLocation.Y >= MaxY)
				{
					goingLeft = true;
					mesh->SetRelativeLocation(FVector(mesh->RelativeLocation.X, MaxY, mesh->RelativeLocation.Z));
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

