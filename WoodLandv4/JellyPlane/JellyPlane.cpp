// Fill out your copyright notice in the Description page of Project Settings.


#include "JellyPlane.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../CrosshairHUD.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
AJellyPlane::AJellyPlane(){
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(PlaneBody);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    // SetReplicates(true);
    // SetReplicateMovement(true);
}

void AJellyPlane::BeginPlay() 
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(ThrottleTimer, this, &AJellyPlane::Throttle, 0.05f, true);
    // CrossHair = Cast<ACrosshairHUD>(UGameplayStatics::GetPlayerController( this->GetOwner(), 0)->GetHUD());
    CurrentHealth = MaxHealth;
} 

float AJellyPlane::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCausor) 
{   
    float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCausor);
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, 500.f);
    if(CurrentHealth<=0){ 
        OnDeath(DamageCausor);
        Destroy();
        }
    return DamageApplied;
}




void AJellyPlane::FireOnhalt(bool bDo, FRotator Rotation, FHitResult Hit) 
{
    if (bDo)
    {
        // DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
        // AActor
        // UE_LOG(LogTemp, Warning, TEXT("name: %s"), Hit.ToString());
        FVector ShotDirection = -Rotation.Vector();
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Flash, Hit.Location, ShotDirection.Rotation(), FVector(5.f));

        // Damage Archi
        FPointDamageEvent DamageEvent(10.f, Hit, ShotDirection, nullptr);
        AActor* HitActor = Hit.GetActor();

        if (HitActor != nullptr) { HitActor->TakeDamage(10.f, DamageEvent, GetController(), this); }
        
    }
}


void AJellyPlane::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
    FVector VNegate = -PlaneBody->GetPhysicsAngularVelocity(FName()) * 2;
    PlaneBody->AddTorqueInDegrees(VNegate, FName(), true);
    // PlaneBody->AddForce(FVector(0.0f, 0.0f, -1500.0f), FName(), true);MoveRight
    CalculateSpeed();
}  


void AJellyPlane::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{   
    // Action Binding... like single key input action
    PlayerInputComponent->BindAction("ThrottleUp", IE_Pressed, this, &AJellyPlane::ThrottleUp);
    PlayerInputComponent->BindAction("ThrottleUp", IE_Released, this, &AJellyPlane::ThrottleUp);
    PlayerInputComponent->BindAction("ThrottleDown", IE_Pressed, this, &AJellyPlane::ThrottleDown);
    PlayerInputComponent->BindAction("ThrottleDown", IE_Released, this, &AJellyPlane::ThrottleDown);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJellyPlane::Fire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AJellyPlane::StopFire);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AJellyPlane::Dash);
    PlayerInputComponent->BindAction("Dash", IE_Released, this, &AJellyPlane::DashStop);
    PlayerInputComponent->BindAction("Special_Ability", IE_Pressed, this, &AJellyPlane::SAbility);
    
    // Axis Binding...like two or more key input action
    PlayerInputComponent->BindAxis("LookUp", this, &AJellyPlane::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &AJellyPlane::Turn);
    PlayerInputComponent->BindAxis("MoveRight", this, &AJellyPlane::MoveRight);
    PlayerInputComponent->BindAxis("Roll", this, &AJellyPlane::Roll);

}

void AJellyPlane::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AJellyPlane, CurrentHealth)
    DOREPLIFETIME(AJellyPlane, ThrottleAmount)
    DOREPLIFETIME(AJellyPlane, Range)
    DOREPLIFETIME(AJellyPlane, bThrottleUp)
    DOREPLIFETIME(AJellyPlane, bThrottleDown)
    DOREPLIFETIME(AJellyPlane, Flash)
}

//replication



void AJellyPlane::spawnEmitters_Implementation(FVector Location, FRotator Rotation) 
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Flash, Location, Rotation, FVector(5.f));
}



void AJellyPlane::serverStopFire_Implementation() 
{
    GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AJellyPlane::Fire, -1, true);
}
bool AJellyPlane::serverStopFire_Validate(){
    return true;
}

void AJellyPlane::serverFire_Implementation() 
{

    GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AJellyPlane::Fire, 0.1, true);

    FVector Location;
    FRotator Rotation;

    GetController()->GetPlayerViewPoint(Location, Rotation);

    FVector End = Location + Rotation.Vector() * 10000.f;
    

    FHitResult Hit;
    bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);
    if (bSuccess)
    {
        // DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
        // AActor
        // UE_LOG(LogTemp, Warning, TEXT("name: %s"), Hit.ToString());
        FVector ShotDirection = -Rotation.Vector();
        spawnEmitters(Hit.Location, ShotDirection.Rotation());

        // Damage Archi
        FPointDamageEvent DamageEvent(10.f, Hit, ShotDirection, nullptr);
        AActor* HitActor = Hit.GetActor();

        if (HitActor != nullptr) { HitActor->TakeDamage(10.f, DamageEvent, GetController(), this); }
        
    }
    // FireOnhalt(bSuccess, Rotation, Hit);
}

bool AJellyPlane::serverFire_Validate() 
{
    return true;
}

void AJellyPlane::serverSAbility_Implementation()
{   
    UE_LOG(LogTemp, Warning, TEXT("well server input's fine"));
    DrawDebugSphere( GetWorld(), GetActorLocation(), 1000.0f, 32, FColor::Red, false, 1.0f );
    // UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Flash, GetActorLocation(), FRotator(0.0f), FVector(5.f));
    // Set what actors to seek out from it's collision channel
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));

    // Ignore any specific actors
    TArray<AActor*> ignoreActors;
    // Ignore self or remove this line to not ignore any
    ignoreActors.Init( this, 1);

    // Array of actors that are inside the radius of the sphere
    TArray<AActor*> outActors;

    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 4000.0f, traceObjectTypes, NULL, ignoreActors, outActors);


    for(AActor* overlappedactor: outActors){
        UE_LOG(LogTemp, Warning, TEXT("OverlappingActor: %s"), *overlappedactor->GetName());
    }
    UE_LOG(LogTemp, Warning, TEXT("OverlappingActor: %d"), outActors.Num());
}

bool AJellyPlane::serverSAbility_Validate() 
{
    return true;
}

void AJellyPlane::serverDash_Implementation() 
{   
    MaxSpeed = 700;
    ThrottleAmount = FMath::Clamp(ThrottleAmount+10, 0, MaxSpeed);
}

void AJellyPlane::serverDashStop_Implementation() 
{
    MaxSpeed = 100;
}


void AJellyPlane::serverTurn_Implementation(float value) 
{
    FRotator DeltaRot = FRotator::ZeroRotator;
    DeltaRot.Yaw = value * sensitivity  * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalRotation(DeltaRot, true);

    // MoveRight(value * sensitivity);
}

void AJellyPlane::serverRoll_Implementation(float value) 
{
    FVector FDirection = PlaneBody->GetForwardVector() * AirControl_Roll * value;

    PlaneBody->AddTorqueInDegrees(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FDirection, 0.2f), FName(), true);
}


void AJellyPlane::serverThrottleUp_Implementation() 
{
    bThrottleUp = !bThrottleUp;
}

void AJellyPlane::serverThrottleDown_Implementation() 
{
    bThrottleDown = !bThrottleDown;
}

void AJellyPlane::serverLookUp_Implementation(float value) 
{   

    FRotator DeltaRot = FRotator::ZeroRotator;
    DeltaRot.Pitch = value * sensitivity  * UGameplayStatics::GetWorldDeltaSeconds(this);

    
    (DeltaRot, true);
    
    // FVector FDirection = PlaneBody->GetRightVector() * AirControl * value * sensitivity;

    // PlaneBody->AddTorqueInDegrees(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FDirection, 0.2f), FName(), true);
}





void AJellyPlane::ThrottleUp() 
{
    serverThrottleUp();
}


void AJellyPlane::ThrottleDown() 
{
    serverThrottleDown();
}
void AJellyPlane::Fire() 
{
    serverFire();
}

void AJellyPlane::StopFire() 
{
    serverStopFire();
}

void AJellyPlane::Dash() 
{
    UE_LOG(LogTemp, Warning, TEXT("i am speed"));
    serverDash();
}

void AJellyPlane::DashStop() 
{   
    UE_LOG(LogTemp, Warning, TEXT("i am speed, BUt no tnow"));
    serverDashStop();
}

void AJellyPlane::SAbility() 
{
    UE_LOG(LogTemp, Warning, TEXT("well input's fine"));
    serverSAbility();
}

float AJellyPlane::GetSpeed() 
{
    return ThrottleAmount;
}

void AJellyPlane::Destroyed_Event() 
{
    Destroy();
}

float AJellyPlane::GetHealth() 
{
    return CurrentHealth;
}

void AJellyPlane::SetHealth(float value) 
{
    CurrentHealth = value;
}


void AJellyPlane::LookUp(float value) 
{

    serverLookUp(value);

    // CrossHair->DragCrossHair(value, 0);
}




void AJellyPlane::Turn(float value) 
{
    serverTurn(value);
    // CrossHair->DragCrossHair(0, value);


}

void AJellyPlane::MoveForward(float value) 
{
    
}

void AJellyPlane::MoveRight(float value) 
{   
	// AddControllerRollInput(value * sensitivity * GetWorld()->GetDeltaSeconds());

    FRotator DeltaRot = FRotator::ZeroRotator;
    DeltaRot.Yaw = value * 10.f  * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalRotation(DeltaRot, true);

    FVector FDirection = PlaneBody->GetUpVector() * AirControl * value;

    PlaneBody->AddTorqueInDegrees(FMath::Lerp(FVector(0.0f, 0.0f, 0.0f), FDirection, 0.2f), FName(), true);
}

void AJellyPlane::Roll(float value) 
{   

    serverRoll(value);


}

void AJellyPlane::Throttle() 
{
    if(bThrottleUp){
        ThrottleAmount = FMath::Clamp(++ThrottleAmount, 0, MaxSpeed);
    }
    else if (bThrottleDown)
        {
            ThrottleAmount = FMath::Clamp(ThrottleAmount-8, 0, MaxSpeed);
        }
        
}

void AJellyPlane::CalculateSpeed() 
{   
    float Speeed = FMath::Clamp(ThrottleAmount * Speed, 0.0f, Speed * 10);
    FVector ForwardVector = PlaneBody->GetForwardVector() * Speeed;
    FVector PVelocity = PlaneBody->GetPhysicsLinearVelocity();
    PlaneBody->SetPhysicsLinearVelocity(FMath::Lerp(PVelocity, ForwardVector, 0.01f));
}

while true:
    n = int(input())
    print(n)
    if(n == 5): break