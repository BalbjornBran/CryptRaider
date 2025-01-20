// ==================================================================
// SecretDoor.h
// Defines a secret door that can be unlocked by specific actions, 
// such as detecting actors with specific tags and allowing dynamic
// interactions in the game world.
//
// Responsibilities:
// - Detect overlapping actors with designated unlocking tags.
// - Activate and deactivate the door based on specific conditions.
// - Support interaction logic for secret door mechanics.
//
// Author: Balbjorn Bran
// Date: December, 27, 2024
// ==================================================================

// Copyright by Balbjorn Bran. All Rights Reserved.
//This code was created for learning purposes.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecretDoor.generated.h"

// Forward declarations to improve compilation speed
class UStaticMeshComponent;
class UMoverComponent;
class UBoxComponent;

/**
 * ASecretDoor represents a door in the game that can open or close 
 * based on the interaction with an overlapping actor.
 */
UCLASS(Blueprintable, Category = "LevelDesign")
class CRYPTRAIDER_API ASecretDoor : public AActor
{
	GENERATED_BODY()

	// ***********************************Private Members*************************************
private:

	/** The 3D mesh of the door that visually represents it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	/** A component responsible for moving the door */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMoverComponent* Mover;

	/** Box component for detecting overlaps with other actors */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;

	/** The actor that is currently overlapping the door's Box component */
	UPROPERTY()
	AActor* CurrentOverlappingActor;

	/** Tag required by the overlapping actor to unlock this door */
	UPROPERTY(EditAnywhere, Category = "Setup", 
		meta = (ToolTip = "The tag required by an overlapping actor to unlock this door"))
	FName TagToUnlock{};

	
	/** Booleano que indica si la puerta está activada */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsActivated = false;

	// *************************************Public Members*************************************
public:

	/** Default constructor that initializes this actor's properties */
	ASecretDoor();
	
	// *************************************Private Methods*************************************
private:

	/**
	 * Called when another actor begins to overlap this door's Box component.
	 * Updates the CurrentOverlappingActor if the conditions are met.
	 */
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	/**
	* Called when an actor stops overlapping this door's Box component.
	* Resets the CurrentOverlappingActor if the conditions are met.
	*/
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/**
	 * Updates the CurrentOverlappingActor with a valid unlocking actor.
	 * @param ValidActor - The actor that satisfies the unlocking conditions.
	 * @return A pointer to the updated CurrentOverlappingActor.
	 */
	AActor* SetUnlockingActor(AActor* ValidActor);



	// *************************************Protected Methods*************************************
protected:

	/** 
	 * Called when the game starts or when the actor is spawned.
	 * Typically used for initialization.
	 */
	virtual void BeginPlay() override;

	// ***************************************Public Methods***************************************
public:

	/** 
	 * Called every frame.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/** 
	 * Retrieves the overlapping actor currently affecting the door.
	 * @return A pointer to the overlapping actor, or nullptr if none.
	 */
	AActor* GetOverlappingActor() const {return CurrentOverlappingActor;}

	/** 
	 * Checks the conditions of the current overlapping actor and manages door behavior (e.g., ActivateDoor).
	 */
	bool ShouldDoorActivate();

	/**
	 * Activates the door and sends the overlapping actor reference to Blueprint.
	 * @param InteractingActor - The actor currently overlapping the door.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void ActivateDoor(AActor* AcceptedActor);
};
