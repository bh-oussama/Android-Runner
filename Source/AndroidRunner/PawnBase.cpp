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
#include "Curves/CurveFloat.h"

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

	// disabling movement component for the customization.
	GetCharacterMovement()->Deactivate();
}

void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APawnBase::OnHitObstacle);
	currMaxAcceleration = maxAcceleration;
	currMaxSpeed = maxSpeed;
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
		calculatedRotation = FRotator(BQuat * AQuat);
		newRotation = UKismetMathLibrary::RInterpTo(
			GetActorRotation(),
			calculatedRotation,
			DeltaTime,
			10.f
		);
		SetActorRotation(newRotation);
		auto camera = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
		if (camera) {
			camera->SetRelativeRotation(FRotator(0.f, 0.f, tiltRate * CAMERA_TILT));
		}
	}

	// adding wheel rotation
	wheel->AddLocalRotation(FRotator(
		GetVelocity().X / (wheelRadius * -4.f),
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

	// update movement speed applying brakes
	float mvnCompSpeed = GetCharacterMovement()->GetMaxSpeed();
	if (bScreenTouched | bDecelerating) {	// applying brakes
		if (mvnCompSpeed > 300.f) {
			GetCharacterMovement()->MaxWalkSpeed = -brakesStrength * DeltaTime + mvnCompSpeed;
			if (mvnCompSpeed < 300.f) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}
	}
	else {	// adding speed to reset speed
		GetCharacterMovement()->MaxWalkSpeed = FMath::Min(
			currMaxSpeed,
			mvnCompSpeed + currMaxAcceleration * DeltaTime
		);
	}

	// deactivating turbo if it has expired
	if (hasBooster && GetWorld()->GetTimeSeconds() > BoosterExpirationTime) {
		hasBooster = false;
		currMaxSpeed = maxSpeed;
		currMaxAcceleration = maxAcceleration;
	}

	if (!hasBooster && currMaxSpeed > maxSpeed + 5.f)
	{
		currMaxSpeed = FMath::FInterpTo(currMaxSpeed, maxSpeed, DeltaTime, maxAcceleration * 1.5f);
	}

	// when health is low control is bad
	if (health < 15.f)
	{
		float noiseFactor = 1.f - health / 15;
		float noise = noiseFactor * 14.f;
		currNoise = FMath::FInterpTo(currNoise, FMath::Cos(GetWorld()->GetTimeSeconds() * 3.14f * noise), DeltaTime, 10.f);
		AddMovementInput(GetActorRightVector(), currNoise);
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

void APawnBase::OnHitObstacle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel1) { // other comp is an obstacle

		// preventing applying damage twice in one frame
		if (HittedSomething) return;
		HittedSomething = true;

		// calculating hit angle
		float result = FVector::DotProduct(Hit.ImpactNormal, FVector(1.f, 0.f, 0.f));
		float acos = FMath::RadiansToDegrees(FMath::Acos(result));
		UE_LOG(LogTemp, Warning, TEXT("hitevent: hit angle: %f"), acos);

		// calculating damage to deal
		float damage2apply = maxDamage * damageByAngleFactorCurve->GetFloatValue(acos);
		UE_LOG(LogTemp, Warning, TEXT("hitevent: damage2apply: %f"), damage2apply);

		// applying damage
		if (armor >= damage2apply) armor -= damage2apply;
		else { health -= damage2apply - armor; armor = 0; }

		// deactivating movement when (health <= 0)
		if (health <= 0)
		{
			GetCharacterMovement()->Deactivate();
			return;
		}

		// applying hit impulse
		if (acos > 100)
		{
			FVector impulse = Hit.ImpactNormal * (BounceForce + GetVelocity().Size()) + FVector(0.f, 0.f, BounceDirectionUpAdditive);
			GetCharacterMovement()->AddImpulse(impulse);
		}
		UE_LOG(LogTemp, Warning, TEXT("hitevent: reporting state: %f/%f"), health, armor);

	}
}
