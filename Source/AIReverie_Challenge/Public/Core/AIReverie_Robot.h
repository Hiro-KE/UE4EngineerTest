// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
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

	/** Hit Distance value represents the minimum distance in between the robot and the object in front of it. The robot will rotate to another direction once it reaches the HitDistance threshold.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Motion")
	float HitDistance = 15.f;
	/** Units the robot can move when moving forward.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Motion")
	float UnitsToMove = 10.f;
	/** Motion and capture timer rate in seconds (Ticking rate delay).*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Motion")
	float TimerRate = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Motion")
	uint8 bDisplayDebug : 1 ;

	uint32 Iteration = 1;
	/** Units above the floor that represent the position of the robot comparing to the floor.*/
	float UnitsAboveGround = 10.f;
	/** Ticking this will make sure that the robot will always be on the surface of the ground by the units above the floor. Useful when the ground is not planar.*/
	uint8 bGroundCheck : 1;

protected:

	UPROPERTY()
	FVector LastLocation;
	UPROPERTY()
	TEnumAsByte<EDirection> LastDirection;
	UPROPERTY()
	FTimerHandle MotionTimer;
	FTimerDelegate MotionDelegate;

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
	void MoveRobot(const float Units = 10.f, const bool bDebug = false);

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
	TEnumAsByte<EDirection> PickBestDirection(const float Degree, const float MaxRange = 25.f, const bool bDebug = false) const;
	UFUNCTION(BlueprintCallable, Category = "AiReverie|Motion")
	void ProcessMotion(const bool bDebug = false);
	UFUNCTION(BlueprintCallable, Category = "AiReverie|Motion")
	void ProcessScene();
	virtual void OnProcessMotion();
	/**
	 * Returns the distance to the closest actor in range with a given rotation to look at from the current (Robot) actor. 
	 * In case there is no actor in range, returns -1 as distance.
	 * Note : Tracing channel is visibility.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AiReverie|Utility")
	float GetTracedDistance(const FVector RotationVector, const float MaxRange = 50.f, const bool bDebug = false) const;
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AiReverie|Utility")
	//void GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, const float MinRecentTime = 0.01f);

};
