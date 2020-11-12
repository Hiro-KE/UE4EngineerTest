// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AIReverie_Robot.h"
#include "AIReverie_Challenge/AIReverie_Challenge.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAIReverie_Robot::AAIReverie_Robot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bGroundCheck = false;
	bDisplayDebug = false;
}

// Called when the game starts or when spawned
void AAIReverie_Robot::BeginPlay()
{
	Super::BeginPlay();
	LastLocation = GetActorLocation();

	MotionDelegate.BindUFunction(this, FName("ProcessMotion"), bDisplayDebug);
	GetWorld()->GetTimerManager().SetTimer(MotionTimer, MotionDelegate, TimerRate, true);
}

// Called every frame
void AAIReverie_Robot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIReverie_Robot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIReverie_Robot::MoveRobot(const float Units /*= 10.f*/, const bool bDebug /*= false*/)
{
	FVector TargetLocation = GetActorForwardVector() * Units + GetActorLocation();

	if (bGroundCheck) {
		if (UWorld* World = GetWorld()) {
			FHitResult OutHit;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			if (World->LineTraceSingleByChannel(OutHit, TargetLocation, TargetLocation + (GetActorUpVector() * (-500.f)), ECC_Visibility, CollisionParams)) {
				if (bDebug) DrawDebugLine(GetWorld(), TargetLocation, TargetLocation + (GetActorUpVector() * (-500.f)), FColor::Green, false, 1, 0, 1);
				if (OutHit.bBlockingHit && OutHit.Component->ComponentHasTag("Ground")) {
					TargetLocation = OutHit.ImpactPoint + UnitsAboveGround;
				}
			}
		}
	}

	SetActorLocation(TargetLocation, true);
}

void AAIReverie_Robot::RotateRobot(const TEnumAsByte<EDirection> Direction, const float Degree)
{
	if (Direction == EDirection::Right) SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + Degree, GetActorRotation().Roll));
	else SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw - Degree, GetActorRotation().Roll));
}

TEnumAsByte<EDirection> AAIReverie_Robot::PickBestDirection(const float Degree, const float MaxRange /*= 25.f*/, const bool bDebug /*= false*/) const
{
	FVector RotationVector_Right = UKismetMathLibrary::GreaterGreater_VectorRotator(GetActorForwardVector(), FRotator(0.f, Degree, 0.f));
	FVector RotationVector_Left = UKismetMathLibrary::GreaterGreater_VectorRotator(GetActorForwardVector(), FRotator(0.f, -Degree, 0.f));

	float DistanceRight = GetTracedDistance(RotationVector_Right, MaxRange, bDebug);
	float DistanceLeft = GetTracedDistance(RotationVector_Left, MaxRange, bDebug);
	if (bDebug) UE_LOG(LogAIReverie_Challenge, Log, TEXT("Robot is going to turn, Furthest point right : %f, Furthest point left : %f"), DistanceRight, DistanceLeft);

	if (DistanceRight == -1.f) return EDirection::Right;
	else if (DistanceLeft == -1.f) return EDirection::Left;
	else if (DistanceRight >= DistanceLeft) return EDirection::Right;
	return EDirection::Left;
}

float AAIReverie_Robot::GetTracedDistance(const FVector RotationVector, const float MaxRange /*= 50.f*/, const bool bDebug /*= false*/) const
{
	if (UWorld * World = GetWorld()) {
		FHitResult OutHit;
		FVector LastPoint = RotationVector * MaxRange + GetActorLocation();
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		if (World->LineTraceSingleByChannel(OutHit, GetActorLocation(), LastPoint, ECC_Visibility, CollisionParams)) {
			if (bDebug) DrawDebugLine(GetWorld(), GetActorLocation(), LastPoint, FColor::Green, false, 1, 0, 1);
			if (OutHit.bBlockingHit) return OutHit.Distance;
		}
	}
	return -1.f;
}



void AAIReverie_Robot::ProcessMotion(const bool bDebug /*= false*/)
{
	// Move the robot if there is nothing in front of it
	float ForwardDistance = GetTracedDistance(GetActorForwardVector(), HitDistance + 1, bDebug);
	//UE_LOG(LogAIReverie_Challenge, Log, TEXT("Forward distance to the next actor is : %f"), ForwardDistance);
	if (ForwardDistance > HitDistance || ForwardDistance == -1.f) {
		MoveRobot(UnitsToMove, bDebug);
	}
	else {
		// Pick a random Degree between 5 and 25
		float Degree = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::RandomFloat(), 0.f, 1.f, 5.f, 25.f);
		// Check if the robot has moved at least twice since the last check
		if (UKismetMathLibrary::Vector_Distance(GetActorLocation(), LastLocation) > (UnitsToMove * 3)) {
			// Picks the best direction depending on the distance to the upcoming object from both sides
			TEnumAsByte <EDirection> Direction = PickBestDirection(Degree, HitDistance * 2, bDebug);
			// Rotate the robot to the Direction and degree
			RotateRobot(Direction, Degree);
			LastLocation = GetActorLocation();
			LastDirection = Direction;
		}
		else {
			RotateRobot(LastDirection, Degree);
			LastLocation = GetActorLocation();
		}
	}
}
