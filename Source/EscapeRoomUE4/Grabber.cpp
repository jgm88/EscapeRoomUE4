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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// LANE TRACE If the physics body is attached move the object we are holding
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector LineTraceEnd;
		CalculateLineTraceEnd(LineTraceEnd);
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	
}

void UGrabber::FindPhysicsHandleComponent()
{
	FString ActorName = GetOwner()->GetName();

	// Check the if the PhysicsHandle is attached
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing UPhysicsHandleComponent"), *ActorName);
	}
}

void UGrabber::SetupInputComponent()
{
	FString ActorName = GetOwner()->GetName();

	// Check the if the InputComponent is attached
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing UInputComponent"), *ActorName);
	}
}

void UGrabber::Grab()
{
	// LINE TRACE and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent * ComponentToGrab = HitResult.GetComponent();
	AActor * ActorHit = HitResult.GetActor();

	// If we hit something then attached a physics handle
	if(ActorHit != nullptr)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::CalculateLineTraceEnd(FVector & LineTraceEnd)
{
	// Get the player view
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);

	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * TraceDistance;
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	FVector LineTraceEnd;
	CalculateLineTraceEnd(LineTraceEnd);

	///// Debug Trace
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false, 0.f, 0.f, 10.f
	//);

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

	return Hit;
}
