// ==================================================================
// Author: Balbjorn Bran
// Date: December, 27, 2024
// ==================================================================

// Copyright by Balbjorn Bran. All Rights Reserved.
//This code was created for learning purposes.

#include "SecretDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MoverComponent.h"

// Sets default values
ASecretDoor::ASecretDoor()
{
	// Enable ticking every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create and configure the door mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	SetRootComponent(DoorMesh); // Set the door mesh as the root component of this actor

	// Create and configure the mover component
	Mover = CreateDefaultSubobject<UMoverComponent>(TEXT("Mover"));

	// Create and configure the box component used for overlap detection
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	// Attach the box component to the root component
	Box->SetupAttachment(RootComponent);

	// Set default tag (can also be set in Unreal Editor)
	TagToUnlock = FName("UnlockingTag");
}

// ********************************************************************************************
// Called when the game starts or when spawned
void ASecretDoor::BeginPlay()
{
	Super::BeginPlay();

	
	if (Box)
	{
		// Bind the OnComponentBeginOverlap event to the HandleBeginOverlap function
		Box->OnComponentBeginOverlap.AddDynamic(this, &ASecretDoor::HandleBeginOverlap);
		// Bind the OnComponentEndOverlap event to the HandleEndOverlap function
		Box->OnComponentEndOverlap.AddDynamic(this, &ASecretDoor::HandleEndOverlap);
	}
}

// ********************************************************************************************
// Called every frame
void ASecretDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check and activate the door based on the current interaction
	if (ShouldDoorActivate())
	{
		ActivateDoor(GetOverlappingActor());
	}
}

// ********************************************************************************************
/**
 * Called when another actor begins overlapping the Box collision component.
 * Updates the CurrentOverlappingActor based on the tag condition.
 */
void ASecretDoor::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Validate the overlapping actor
	if (!OtherActor || OtherActor == this) 
	{
		return;
	}

	if (OtherActor->ActorHasTag(TagToUnlock) && OtherActor->ActorHasTag("Grabbed")) 
	{
		CurrentOverlappingActor = OtherActor;
	}
}

// ********************************************************************************************
/**
 * Called when another actor ends overlapping the Box collision component.
 * Resets the CurrentOverlappingActor if the actor ending overlap matches the unlocking criteria.
 */
void ASecretDoor::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Validate if the actor that stopped overlapping is the same as the CurrentOverlappingActor
	if (OtherActor == CurrentOverlappingActor)
	{
		// Reset the reference to nullptr as the actor is no longer overlapping
		CurrentOverlappingActor = nullptr;
	}
}

// ********************************************************************************************
/**
 * Updates the CurrentOverlappingActor to an unlocking actor if conditions are fulfilled.
 * Used for assigning a new actor that satisfies the unlocking requirements.
 */
AActor* ASecretDoor::SetUnlockingActor(AActor* ValidActor)
{
	// Assign a valid actor as the current interacting actor
	if (ValidActor)
	{
		CurrentOverlappingActor = ValidActor;
		return CurrentOverlappingActor;
	}
	else
	{
		// Return nullptr if no valid actor is found
		return nullptr;
	}
}

// ********************************************************************************************
/**
 * Checks the overlapping actor and determines whether the door should be activated.
 */
bool ASecretDoor::ShouldDoorActivate()
{
	// Check if the door is already activated
	if (bIsActivated || !CurrentOverlappingActor)
	{
		return false; 
	}

	if (CurrentOverlappingActor->ActorHasTag(TagToUnlock) && 
		!CurrentOverlappingActor->ActorHasTag(FName("Grabbed")))
	{
		bIsActivated = true; 
		return true; 
	}

	// If no valid actor is found, return false
	return false;
}

