// Copyright : Jed Fakhfekh

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Global")
	float TimerRate = 1.f;
	/** Whether display the Logs and Traces or not */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Global")
	uint8 bDisplayDebug : 1 ;
	/** Tag of the actors to be ignored and to not add them to the list in the saved file */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIReverie|Scene")
	FName IgnoredActorsTag = "NoFileSave";
	
	/** Process iteration counter (used for file naming) */
	uint32 Iteration = 1;

protected:
	/** Represents the last location where the robot has rotated. Used to have less bad iterations to figure out the direction to take.*/
	UPROPERTY()
	FVector LastLocation;
	/** Represents the last rotation the robot has picked. Used to have less bad iterations.*/
	UPROPERTY()
	TEnumAsByte<EDirection> LastDirection;
	/** Timer handles */
	UPROPERTY()
	FTimerHandle MotionTimer;
	UPROPERTY()
	FTimerHandle CaptureTimer;
	FTimerDelegate MotionDelegate;

public:
	// Sets default values for this pawn's properties
	AAIReverie_Robot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** Move the robot for a given units number.*/
	UFUNCTION(BlueprintCallable, Category = "AIReverie|Motion")
	void MoveRobot(const float Units = 10.f, const bool bDebug = false);
	/** Rotate the robot in a certain direction with a given degree.*/
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
	/**
	 * ProcessMotion will move and rotate the robot in the environment depending on the variables set in the class default.
	 * @param bDebug If the debugging traces and text will be displayed or not.
	 */
	UFUNCTION(BlueprintCallable, Category = "AiReverie|Motion")
	void ProcessMotion(const bool bDebug = false);
	/** Called after processing the robot's motion (Native implementation)*/
	virtual void OnProcessMotion();
	/** Called after processing the robot's motion */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnProcessMotion"), Category = "AiReverie|Motion")
	void OnProcessMotion_BP();
	/** 
	 * Call this in order to take a screenshot of the current screen and create a file of all the actors present in the scene.
	 * Note : Make sure you have a valid controller possessing this pawn in order to fill properly the list of actors.
	 */
	UFUNCTION(BlueprintCallable, Category = "AiReverie|Scene")
	void ProcessScene();
	/** Called after processing the robot's scene capture and files creation (Native implementation)*/
	virtual void OnProcessScene();
	/** Called after processing the robot's scene capture and files creation */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnProcessScene"), Category = "AiReverie|Scene")
	void OnProcessScene_BP();
	/**
	 * Returns the distance to the closest actor in range with a given rotation to look at from the current (Robot) actor. 
	 * In case there is no actor in range, returns -1 as distance.
	 * Note : Tracing channel is visibility.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AiReverie|Utility")
	float GetTracedDistance(const FVector RotationVector, const float MaxRange = 50.f, const bool bDebug = false) const;
};
