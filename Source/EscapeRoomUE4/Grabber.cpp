// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoomUE4.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("GRABBER HERE"));
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString ActorName = GetOwner()->GetName();

	// Check the if the PhysicsHandle is attached
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing UPhysicsHandleComponent"), *ActorName);
	}

	// Check the if the InputComponent is attached
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing UInputComponent"), *ActorName);
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get the player view
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation, 
		PlayerViewPointRotation
	);

	/// Debug Trace
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * TraceDistance;
	DrawDebugLine(
		GetWorld(), 
		PlayerViewPointLocation, 
		LineTraceEnd, 
		FColor(255, 0, 0),
		false, 0.f, 0.f, 10.f
	);

	/// Setup Query Params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		Hit, PlayerViewPointLocation, LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// Check what we hit
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		FString ActorName = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorName);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("EXECUTED GRAB"));
}