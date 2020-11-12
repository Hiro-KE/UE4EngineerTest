// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/AIReverieBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAIReverieBlueprintLibrary::GetSeenActors(APawn* Pawn, TArray<AActor*>& ResultActors, const float Tolerance /*= 0.01f*/)
{
	TArray<AActor*> AllActors;
	ResultActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(Pawn, AActor::StaticClass(), AllActors);
	AController* Controller = Pawn->GetController();
	for (AActor* TempActor : AllActors)
	{
		if (TempActor->WasRecentlyRendered(Tolerance) && TempActor != Pawn && Controller) {
			if (Controller->LineOfSightTo(TempActor, Pawn->GetActorLocation()))
				ResultActors.AddUnique(TempActor);
		}
	}
}
