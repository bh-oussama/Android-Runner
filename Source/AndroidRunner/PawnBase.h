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

	/* 
	*determines the maximum speed with which the pawn should mouve
	*this variable changes in runtime to give the pawn its maximum speed and currMaxSpeed is used to follow MaxSpeed and add Turbo Boost when it is applied	
	*/
	UPROPERTY(EditAnywhere, Category = "Movement")
		float maxSpeed = 1800.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float maxAcceleration = 200.f;

	UPROPERTY(EditAnywhere, Category = "DamageSimulation")
		UCurveFloat* damageByAngleFactorCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "DamageSimulation")
		float maxDamage = 25;

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetHealth() { return health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetArmor() { return armor; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsPlaying() { return isPlaying; }

	UFUNCTION(BlueprintCallable)
		void SetIsPlaying(bool isPlaying) { this->isPlaying = isPlaying; }

	float health = 100;
	float armor = 0;
	float armorExpirationTime = 0.f;
	bool hasBooster = false;
	float BoosterExpirationTime = 0.f;
	float currMaxSpeed = 0.f;
	float currMaxAcceleration = 0.f;
	bool isPlaying = false;
	float currNoise = 0;

private:
	float deltaTime = 0.f;
	FVector lastLocation = FVector(0);
	bool bScreenTouched = false;
	bool bDecelerating = false;
	float currentInclinaison = 0.f;

	// this variable is only used to prevent calling OnHitObstacle more than 1 time.
	bool HittedSomething = false;
};
