// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIReverie_Robot.generated.h"

UENUM(BlueprintType)
enum EDirection {
	Right,
	Left
};

UCLASS()
class AIREVERIE_CHALLENGE_API AAIReverie_Robot : public APawn
{
	GENERATED_BODY()

public:

	/** Units above the floor represent the eye height of the robot (usually 10).*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Data")
	float UnitsAboveFloor = 10.f;
	/** Ticking this will make sure that the robot will always be on the surface of the ground by the units above the floor. Useful when the ground is not planar.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Data")
	uint8 bGroundCheck : 1;

public:
	// Sets default values for this pawn's properties
	AAIReverie_Robot();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AIReverie|Motion")
	void MoveRobot(const float Units = 10.f);

	UFUNCTION(BlueprintCallable, Category = "AIReverie|Motion")
	void RotateRobot(const TEnumAsByte<EDirection> Direction, const float Degree);
	/**
	 * Picks the best direction from right and left depending on the distance to the next object
	 * in order to avoid facing another obstacle again soon or being stuck in a corner for a couple of iterations.
	 * @param Degree The degree value will be tested on both sides of the Robot (Right and Left) depending on its current rotation in the world space.
	 * @param MaxRange The max range is the furthest point to check from the current position of the robot.
	 * @return Returns which direction is the best to choose (Returns right in case both are higher than the given MaxRange).
	 */
	UFUNCTION(BlueprintCallable, Category = "AiReverie|Motion")
	TEnumAsByte<EDirection> PickBestDirection(const float Degree, const float MaxRange = 25.f) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AiReverie|Utility")
	float CheckDistance(const float MaxRange = 50.f, const bool bDebug = false) const;
};
