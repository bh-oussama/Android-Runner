// Fill out your copyright notice in the Description page of Project Settings.

#include "MagnetBase.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "MyLib.h"
#include "EndlessGM.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AMagnetBase::PawnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto gm = MyLib::GetEndlessGM(GetWorld());
	gm->MagnetRange = Range;
	gm->MagnetPower = Power;
	gm->MagnetSound = MagneticSound;
	gm->bIsMagnetActivated = true;
	gm->MagnetDeactivationTime = GetWorld()->GetTimeSeconds() + Duration;
	if (MagneticSound != nullptr) {
		if (gm->AudioSource)
			gm->AudioSource->DestroyComponent();
		gm->AudioSource = UGameplayStatics::SpawnSound2D(
			GetWorld(),
			MagneticSound,
			1.f,
			1.f,
			0.f
		);
	}
}
