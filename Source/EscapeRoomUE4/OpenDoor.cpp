// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoomUE4.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	AOwner = GetOwner();
	// Get The main Pawn
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger Volume
	// In the ActorThatOpens is in
	if(PressurePlate && ActorThatOpens)
	{ 
		if (PressurePlate->IsOverlappingActor(ActorThatOpens))
		{
			OpenDoor();
			LastTimeOpenDoor = GetWorld()->GetTimeSeconds();
		}
		else if(GetWorld()->GetTimeSeconds() - LastTimeOpenDoor > DoorCloseDelay)
		{
			CloseDoor();
		}
	}
	
}

void UOpenDoor::OpenDoor()
{
	AOwner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}
void UOpenDoor::CloseDoor()
{
	AOwner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


