// Fill out your copyright notice in the Description page of Project Settings.

#include "FixToolBase.h"
#include "Engine/World.h"
#include "PawnBase.h"

void AFixToolBase::PawnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto playerPawn = Cast<APawnBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	playerPawn->health = FMath::Min<float>(playerPawn->health + FixAmount, 100.f);
	Destroy();
}
