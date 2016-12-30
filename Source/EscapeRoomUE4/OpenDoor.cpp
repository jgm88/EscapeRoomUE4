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
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing PressurePlate"), *AOwner->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PressurePlate) return;
	// Poll the trigger Volume
	if(PressurePlate)
	{ 
		if (GetTotalMassOfActorsOnPlate() > TriggerMass)
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

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (!PressurePlate) return TotalMass;

	// Calculate mass of all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on preassure plate"), *Actor->GetName());
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	AOwner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}
void UOpenDoor::CloseDoor()
{
	AOwner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


