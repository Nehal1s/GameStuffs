// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BasePlane/VPlane.h"
#include "JellyPlane.generated.h"

/**
 * 
 */


UCLASS()
class WOODLANDV4_API AJellyPlane : public AVPlane
{
	GENERATED_BODY()
	
public:
	AJellyPlane();

private:
	class ACrosshairHUD* CrossHair;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;

	FTimerHandle Timer;


	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Movements")
	float TurnRate = 100;

	UPROPERTY(EditAnywhere, Category = "Movements")
	float Speed = 100;

	UPROPERTY(EditAnywhere, Category = "Movements")
	float Gravity = 100;

	UPROPERTY(EditAnywhere, Category = "Movements")
	float Accelaration = 100;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Movement Variables 
	bool bThrottleUp = false;
	bool bThrottleDown = false;

	// Movement Variables
	int ThrottleAmount = 50;
	UPROPERTY(EditAnywhere, Category = "Movements")
	float AirControl = 2500.f;
	UPROPERTY(EditAnywhere, Category = "Movements")
	float sensitivity = 1;
	

	// Movement Fucntion
	void ThrottleUp();
	void ThrottleUp1();
	void ThrottleDown();
	void ThrottleDown1();
	void Fire();
	void Jump();
	UFUNCTION(BlueprintCallable, Category="Movements")
	float GetSpeed();

	void LookUp(float value);
	void Turn(float value);
	void MoveForward(float value);
	void MoveRight(float value);
	void Roll(float value);

	void Throttle();
	void CalculateSpeed();


};
