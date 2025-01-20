// ==================================================================
// MoverComponent.h
// Defines a reusable movement component that handles the motion of
// actors in the game, allowing for dynamic movement behavior.
//
// Responsibilities:
// - Move actors along defined paths or between specific locations.
// - Configure movement speed and other related parameters.
// - Provide support for smooth and controlled movement transitions.
//
// Author: Balbjorn Bran
// Date: December, 27, 2024
// ==================================================================

// Copyright by Balbjorn Bran. All Rights Reserved.
//This code was created for learning purposes.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoverComponent();

	//Called to move the door
	UFUNCTION(BlueprintCallable)
	void SetShouldMove (bool NewShouldMove);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	float MoveTime = 4;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;
	
	FVector OriginalLocation;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
