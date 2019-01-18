// Fill out your copyright notice in the Description page of Project Settings.

#include "BoostBase.h"
#include "Engine/World.h"
#include "PawnBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABoostBase::PawnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto playerPawn = Cast<APawnBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!playerPawn->hasBooster)
	{
		playerPawn->hasBooster = true;
		playerPawn->currMaxSpeed += AddedSpeedLimit;
		playerPawn->currMaxAcceleration += Acceleration;
	}
	playerPawn->BoosterExpirationTime = GetWorld()->GetTimeSeconds() + Duration;
	Destroy();
}
