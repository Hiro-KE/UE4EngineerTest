// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AIReverieBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AIREVERIE_CHALLENGE_API UAIReverieBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Returns the actors that are rendered in the current screen with a given tolerance.
	 * Note : Requires a valid pawn having a controller.
	 * @param Tolerance  How many seconds ago the actor last render time can be and still count as having been "recently" rendered.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AiReverie|Utility")
	static void GetSeenActors(APawn* Pawn, TArray<AActor*>& ResultActors, const float Tolerance = 0.0f);
};
