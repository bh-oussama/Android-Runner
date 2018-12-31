// Fill out your copyright notice in the Description page of Project Settings.

#include "CoinBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EndlessGM.h"
#include "MyLib.h"

void ACoinBase::BeginPlay()
{
	Super::BeginPlay();
	gm = MyLib::GetEndlessGM(GetWorld());
	player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void ACoinBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (gm->bIsMagnetActivated) {
		if (player) {
			if ((GetActorLocation() - player->GetActorLocation()).Size() <= gm->MagnetRange) {
				SetActorLocation(
					UKismetMathLibrary::VInterpTo(
						GetActorLocation(),
						player->GetActorLocation(),
						DeltaTime,
						gm->MagnetPower
					)
				);
			}
		}
	}
}

void ACoinBase::PawnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APickableBase::PawnOverlap(
		OverlappedComponent,
		OtherActor,
		OtherComp,
		OtherBodyIndex,
		bFromSweep,
		SweepResult
	);
	gm->CollectedCoinsCount++;
	Destroy();
}

