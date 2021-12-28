// Fill out your copyright notice in the Description page of Project Settings.


#include "JellyPlane.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "../CrosshairHUD.h"
AJellyPlane::AJellyPlane(){
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(PlaneBody);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

}

void AJellyPlane::BeginPlay() 
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(Timer, this, &AJellyPlane::Throttle, 0.05f, true);
    CrossHair = Cast<ACrosshairHUD>(UGameplayStatics::GetPlayerController( this->GetOwner(), 0)->GetHUD());
} 

void AJellyPlane::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
    FVector VNegate = -PlaneBody->GetPhysicsAngularVelocity(FName()) * 0.5;
    PlaneBody->AddTorqueInDegrees(VNegate, FName(), true);
    PlaneBody->AddForce(FVector(0.0f, 0.0f, -1500.0f), FName(), true);
    CalculateSpeed();
}  


void AJellyPlane::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{
    PlayerInputComponent->BindAction("ThrottleUp", IE_Pressed, this, &AJellyPlane::ThrottleUp);
    PlayerInputComponent->BindAction("ThrottleUp", IE_Released, this, &AJellyPlane::ThrottleUp);
    PlayerInputComponent->BindAction("ThrottleDown", IE_Pressed, this, &AJellyPlane::ThrottleDown);
    PlayerInputComponent->BindAction("ThrottleDown", IE_Released, this, &AJellyPlane::ThrottleDown);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJellyPlane::Fire);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AJellyPlane::Jump);
    

    PlayerInputComponent->BindAxis("LookUp", this, &AJellyPlane::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AJellyPlane::Turn);
    PlayerInputComponent->BindAxis("MoveForward", this, &AJellyPlane::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AJellyPlane::MoveRight);
    PlayerInputComponent->BindAxis("Roll", this, &AJellyPlane::Roll);

}

void AJellyPlane::ThrottleUp() 
{
    bThrottleUp = !bThrottleUp;
}


void AJellyPlane::ThrottleDown() 
{
    bThrottleDown = !bThrottleDown;
}


void AJellyPlane::Fire() 
{
    
}

void AJellyPlane::Jump() 
{
    
}

float AJellyPlane::GetSpeed() 
{
    return ThrottleAmount;
}

void AJellyPlane::LookUp(float value) 
{

    FVector FDirection = PlaneBody->GetRightVector() * AirControl * value * sensitivity;

    PlaneBody->AddTorqueInDegrees(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FDirection, 0.1f), FName(), true);
    CrossHair->DragCrossHair(value, 0);
}

void AJellyPlane::Turn(float value) 
{
    MoveRight(value * sensitivity);
    CrossHair->DragCrossHair(0, value);

}

void AJellyPlane::MoveForward(float value) 
{
    
}

void AJellyPlane::MoveRight(float value) 
{
    FVector FDirection = PlaneBody->GetUpVector() * AirControl * value;

    PlaneBody->AddTorqueInDegrees(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FDirection, 0.1f), FName(), true);
}

void AJellyPlane::Roll(float value) 
{   

    FVector FDirection = PlaneBody->GetForwardVector() * AirControl * value;

    PlaneBody->AddTorqueInDegrees(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FDirection, 0.1f), FName(), true);

}

void AJellyPlane::Throttle() 
{
    if(bThrottleUp){
        ThrottleAmount = FMath::Clamp(++ThrottleAmount, 0, 100);
        UE_LOG(LogTemp, Warning, TEXT(" Throttle : %d"), ThrottleAmount);
    }
    else if (bThrottleDown)
        {
            ThrottleAmount = FMath::Clamp(--ThrottleAmount, 0, 100);
            UE_LOG(LogTemp, Warning, TEXT(" Throttle : %d"), ThrottleAmount);
        }
        
}

void AJellyPlane::CalculateSpeed() 
{   
    float Speeed = FMath::Clamp(ThrottleAmount * 250.0f, 0.0f, 25000.0f);
    FVector ForwardVector = PlaneBody->GetForwardVector() * Speeed;
    FVector PVelocity = PlaneBody->GetPhysicsLinearVelocity();
    PlaneBody->SetPhysicsLinearVelocity(FMath::Lerp(PVelocity, ForwardVector, 0.01f));
}
