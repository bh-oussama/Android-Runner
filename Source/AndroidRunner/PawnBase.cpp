// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

APawnBase::APawnBase()
{
	PrimaryActorTick.bCanEverTick = true;

	wheel = CreateDefaultSubobject<UStaticMeshComponent>("Wheel");
	wheel->SetupAttachment(RootComponent);
	wheel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	wheel->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	wheel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	wheel->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	cover = CreateDefaultSubobject<UStaticMeshComponent>("Cover");
	cover->SetupAttachment(RootComponent);
	cover->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	cover->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	cover->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	cover->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	cover->SetNotifyRigidBodyCollision(true);
}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APawnBase::OnHitObstacle);
}

void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HittedSomething = false;
	deltaTime = DeltaTime;
	lastLocation = GetActorLocation();

	// adding forward movement
	AddMovementInput(FVector(1.f, 0.f, 0.f));

	// update pawn's rotation by applying tilt
	FVector location = GetActorLocation();
	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(
		hitResult,
		location,
		location - FVector(0.f, 0.f, TRACE_LENGTH),
		ECollisionChannel::ECC_Visibility
	)) {
		FRotator newRotation, calculatedRotation, tiltRotation, staticRotation;
		float tiltRate = GetVelocity().Y * -0.005f;
		tiltRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(
			FVector(1.f, 0.f, 0.f),
			UKismetMathLibrary::FInterpTo(
				currentInclinaison,
				tiltRate * maxInclinaison,
				DeltaTime,
				inclinaisonAcceleration
			)
		);
		staticRotation = UKismetMathLibrary::MakeRotFromXZ(FVector(1.f, 0.f, 0.f), hitResult.Normal);
		FQuat AQuat = FQuat(staticRotation);
		FQuat BQuat = FQuat(tiltRotation);
		calculatedRotation = FRotator(BQuat*AQuat);
		newRotation = UKismetMathLibrary::RInterpTo(
			GetActorRotation(),
			calculatedRotation,
			DeltaTime,
			10.f
		);
		SetActorRotation(newRotation);
		auto camera = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
		if (camera) {
			camera->SetRelativeRotation(FRotator(0.f, 0.f, tiltRate*CAMERA_TILT));
		}
	}

	// adding wheel rotation
	wheel->AddLocalRotation(FRotator(
		GetVelocity().X / (wheelRadius*-4.f),
		0.f,
		0.f
	));

	// updating bScreenTouched state
	float x, y;
	GetWorld()->GetFirstPlayerController()->GetInputTouchState(
		ETouchIndex::Touch1,
		x,
		y,
		bScreenTouched
	);

	// applying brakes
	float mvnCompSpeed = GetCharacterMovement()->GetMaxSpeed();
	if (bScreenTouched || bDecelerating) {
		if (mvnCompSpeed > 300.f) {
			GetCharacterMovement()->MaxWalkSpeed = -brakesStrength * DeltaTime + mvnCompSpeed;
			if (mvnCompSpeed < 300.f) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = FMath::Min(
			maxSpeed,
			mvnCompSpeed + maxAcceleration * DeltaTime
		);
	}
}

void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Tilt", this, &APawnBase::Tilt);
	PlayerInputComponent->BindAxis("Decelerate", this, &APawnBase::Decelerate);
}

void APawnBase::Tilt(float Rate)
{
	// input from keyboard
	AddMovementInput(
		FVector(0.f, 1.f, 0.f),
		tiltSensitivity * Rate
	);

	// input from phone
	FVector vTilt, vRotation, vGravity, vAcceleration;
	GetWorld()->GetFirstPlayerController()->GetInputMotionState(vTilt, vRotation, vGravity, vAcceleration);
	AddMovementInput(
		FVector(0.f, 1.f, 0.f),
		-tiltSensitivity * vTilt.X
	);
}

void APawnBase::Decelerate(float Rate)
{
	bDecelerating = (Rate < 0.f);
}

void APawnBase::OnHitObstacle(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (HittedSomething) return;
	HittedSomething = true;
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel1) {
		// TODO: deal damage on hit
		//			applied damage should be calculated from movement direction & velocity

		// deactivate movements if the player has no more armor & health
		if (armor == 0 && health <= 0)
		{
			GetCharacterMovement()->Deactivate();
			return;
		}
		FVector impulse = Hit.ImpactNormal* (BounceForce + GetVelocity().Size()) + FVector(0.f, 0.f, BounceDirectionUpAdditive);
		/*
		UKismetSystemLibrary::DrawDebugLine(
			GetWorld(),
			Hit.ImpactPoint,
			Hit.ImpactPoint + impulse,
			FLinearColor::Blue,
			10.f,
			5.f
		);
		*/
		GetCharacterMovement()->AddImpulse(impulse);
		armor = 0;
	}
}

