// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOMUE4_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	
private:

	/// Internal properties
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	UPROPERTY(EditAnywhere)
	float TraceDistance = 100.f;	// Max distance to grab things
	void CalculateLineTraceEnd(FVector & LineTraceEnd);

	/// External properties
	UPhysicsHandleComponent * PhysicsHandle = nullptr;
	void FindPhysicsHandleComponent();

	UInputComponent * InputComponent = nullptr;
	void SetupInputComponent();

	/// Method to pull objects
	void Grab();

	/// Method to release pulled objects
	void Release();
	
	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
