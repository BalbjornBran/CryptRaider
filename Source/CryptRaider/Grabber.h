// ==================================================================
// Grabber.h
// Class responsible for "grabbing" objects in the game.
//
// This class handles the logic for:
// - Identifying and selecting interactable objects.
// - Physically grabbing and releasing objects.
// - Integration with Unreal Engine's collision and physics systems.
//
// Author: Balbjorn Bran
// Date: December, 27, 2024
// ==================================================================

// Copyright by Balbjorn Bran. All Rights Reserved.
//This code was created for learning purposes.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Grab();
	UFUNCTION(BlueprintCallable)
	void Release();

private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 150;
	UPROPERTY(EditAnywhere)
	float GrabRadius = 20;
	UPROPERTY(EditAnywhere)
	float HoldDistance = 100;

	UPhysicsHandleComponent* GetPhysicsHandleComponent() const;
	
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};

