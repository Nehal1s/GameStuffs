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

	UPROPERTY(Replicated, EditAnywhere, Category = "Camera")
	UParticleSystem* Flash;
	UPROPERTY(Replicated)
	FTimerHandle ThrottleTimer;
	UPROPERTY(Replicated)
	FTimerHandle FireTimer;


	virtual void BeginPlay() override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCausor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void DecreaseHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath(AActor* OtherActor);



// Server Function
	UFUNCTION(Server, Reliable, WithValidation)
	void serverFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void serverSAbility();

	UFUNCTION(Server, Reliable, WithValidation)
	void serverStopFire();

	UFUNCTION(Server, Reliable)
	void serverThrottleUp();

	UFUNCTION(Server, Reliable)
	void serverThrottleDown();

	UFUNCTION(Server, Reliable)
	void serverTurn(float value);

	UFUNCTION(Server, Reliable)
	void serverRoll(float value);
	
	UFUNCTION(Server, Reliable)
	void serverLookUp(float value);
	
	UFUNCTION(Server, Reliable)
	void serverDash();

	UFUNCTION(Server, Reliable)
	void serverDashStop();

	void FireOnhalt(bool bDo, FRotator Rotation, FHitResult Hit);



	UFUNCTION(NetMulticast, Reliable)
	void spawnEmitters(FVector Location, FRotator Rotation);

	UPROPERTY(BlueprintReadOnly)
	AActor* DeathCausor;

	//i don;t know what am i doing
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



private:
	// Movement Variables 
	UPROPERTY(Replicated)
	bool bThrottleUp = false;
	UPROPERTY(Replicated)
	bool bThrottleDown = false;

	// Movement Variables
	UPROPERTY(Replicated)
	int ThrottleAmount = 0;
	int MaxSpeed = 100;
	UPROPERTY(Replicated, EditAnywhere, Category = "Movements")
	float AirControl = 2500.f;
	UPROPERTY(Replicated, EditAnywhere, Category = "Movements")
	float AirControl_Roll = 2500.f;
	UPROPERTY(Replicated, EditAnywhere, Category = "Movements")
	float sensitivity = 1;	
	UPROPERTY(EditAnywhere, Category = "Movements")
	float MaxHealth = 500;	
	UPROPERTY(Replicated,EditAnywhere, Category = "Movements")
	float CurrentHealth;
	UPROPERTY(Replicated, EditAnywhere, Category = "Movements")
	float Range = 10000;	
	

	// Movement Fucntion
	//  Client Functions
	void ThrottleUp();
	void ThrottleUp1();
	void ThrottleDown();
	void ThrottleDown1();
	void Fire();
	void StopFire();
	void Dash();
	void DashStop();
	void SAbility();
	UFUNCTION(BlueprintCallable, Category="Movements")
	float GetSpeed();
	UFUNCTION(BlueprintCallable, Category="Destro")
	void Destroyed_Event();
	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealth();
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetHealth(float value);

	//Special ABility shit




	void LookUp(float value);
	void Turn(float value);
	
	void MoveForward(float value);
	void MoveRight(float value);
	void Roll(float value);

	void Throttle();
	void CalculateSpeed();


};
