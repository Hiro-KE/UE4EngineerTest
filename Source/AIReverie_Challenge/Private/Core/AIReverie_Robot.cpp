// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AIReverie_Robot.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAIReverie_Robot::AAIReverie_Robot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIReverie_Robot::BeginPlay()
{
	Super::BeginPlay();
	
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

void AAIReverie_Robot::MoveRobot(const float Units /*= 10.f*/)
{
	  SetActorLocation(GetActorForwardVector() * Units + GetActorLocation());
}

void AAIReverie_Robot::RotateRobot(const TEnumAsByte<EDirection> Direction, const float Degree)
{
	UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::RandomFloat(), 0.f, 1.f, 5.f, 25.f);
}

TEnumAsByte<EDirection> AAIReverie_Robot::PickBestDirection(const float Degree, const float MaxRange /*= 25.f*/) const 
{
	return EDirection::Right;
}

float AAIReverie_Robot::CheckDistance(const float MaxRange /*= 50.f*/, const bool bDebug /*= false*/) const 
{
	if (UWorld * World = GetWorld()) {
		FHitResult OutHit;
		FVector LastPoint = GetActorForwardVector() * MaxRange + GetActorLocation();
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		if (World->LineTraceSingleByChannel(OutHit, GetActorLocation(), LastPoint, ECC_WorldStatic, CollisionParams))
		{
			if (bDebug) DrawDebugLine(GetWorld(), GetActorLocation(), LastPoint, FColor::Green, false, 1, 0, 1);
			if (OutHit.bBlockingHit)
			{
				if (GEngine) {

					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
					return OutHit.Distance;
				}
			}
		}
	}
	return 0.f;
}
