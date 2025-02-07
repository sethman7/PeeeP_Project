// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	APPPlayerState();

	void SetSpawnLocation(FVector Location) { SapwnLocation = Location; }
	FVector GetSpawnLocation() { return SapwnLocation; }

private:

	FVector SapwnLocation;

};
