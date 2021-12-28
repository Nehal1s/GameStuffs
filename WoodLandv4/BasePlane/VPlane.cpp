// Fill out your copyright notice in the Description page of Project Settings.


#include "VPlane.h"

// Sets default values
AVPlane::AVPlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PlaneBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Mesh"));
	PlaneBody->SetupAttachment(RootComponent);

	R_Muzzel = CreateDefaultSubobject<USceneComponent>(TEXT("R_Muzzel"));
	R_Muzzel->SetupAttachment(PlaneBody);

	L_Muzzel = CreateDefaultSubobject<USceneComponent>(TEXT("L_Muzzel"));
	L_Muzzel->SetupAttachment(PlaneBody);
}

// Called when the game starts or when spawned
void AVPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}