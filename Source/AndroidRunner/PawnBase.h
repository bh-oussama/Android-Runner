// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnBase.generated.h"

#define		TRACE_LENGTH							100.f
#define		CAMERA_TILT								0.75f
#define		PLAYER_MINIMAL_BOUNCE_FORCE				800000.f

UCLASS()
class ANDROIDRUNNER_API APawnBase : public ACharacter
{
	GENERATED_BODY()

public:
	APawnBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* wheel = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* cover = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float BounceForce = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float BounceDirectionUpAdditive = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float maxInclinaison = 7.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float inclinaisonAcceleration = 80.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float tiltSensitivity = 5.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float tiltRateScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float wheelRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float brakesStrength = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float maxSpeed = 1800.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float maxAcceleration = 200.f;

	UFUNCTION()
		void Tilt(float Rate);

	UFUNCTION()
		void Decelerate(float Rate);

	UFUNCTION()
		void OnHitObstacle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class UStaticMeshComponent* GetWheel() { return wheel; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class UStaticMeshComponent* GetCover() { return cover; }

	int32 health = 5;
	int32 armor = 0;
	float armorExpirationTime = 0.f;
private:
	float deltaTime = 0.f;
	FVector lastLocation = FVector(0);
	bool bScreenTouched = false;
	bool bDecelerating = false;
	float currentInclinaison = 0.f;

	// this variable is only used to prevent calling OnHitObstacle more than 1 time.
	bool HittedSomething = false;
};
