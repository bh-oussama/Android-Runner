// Fill out your copyright notice in the Description page of Project Settings.

#include "ShieldBase.h"
#include "Engine/World.h"
#include "PawnBase.h"

void AShieldBase::PawnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto playerPawn = Cast<APawnBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	playerPawn->armor = FMath::Max(Armor, playerPawn->armor);
	playerPawn->armorExpirationTime = GetWorld()->GetTimeSeconds() + Duration;
	Destroy();
}
