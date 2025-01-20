// ==================================================================
// Author: Balbjorn Bran
// Date: December, 27, 2024
// ==================================================================

// Copyright by Balbjorn Bran. All Rights Reserved.
//This code was created for learning purposes.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();		
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

// The Grab function allows the player to grab a physics object within reach. 
// If an object is grabbed, it updates its location every frame. If no object is grabbed, 
// it checks for a nearby object using a sweep and grabs it if possible.
void UGrabber::Grab()
{
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandleComponent();
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	else
	{
		FHitResult HitResult;
		bool HasHit = GetGrabbableInReach(HitResult);
		if (HasHit)
		{
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			HitComponent->SetSimulatePhysics(true);
			HitComponent->WakeAllRigidBodies();
			AActor* HitActor = HitResult.GetActor();
			HitActor->Tags.Add("Grabbed");
			HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				HitComponent,
				NAME_None,
				HitResult.ImpactPoint,
				GetComponentRotation());
		}
	}

}


// The Release function releases the currently grabbed physics object, removes the "Grabbed" tag, and stops its physics interaction.
void UGrabber::Release()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "Released");
	
	
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandleComponent();
	

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
	
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandleComponent() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	//DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
}

