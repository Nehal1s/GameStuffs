// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VPlane.generated.h"

UCLASS()
class WOODLANDV4_API AVPlane : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* PlaneBody;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USceneComponent* R_Muzzel;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USceneComponent* L_Muzzel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
