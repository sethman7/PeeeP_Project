// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPCleaningRobot.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPCleaningRobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPCleaningRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

UFUNCTION()
	virtual void NotifyHit(
		UPrimitiveComponent* MyComp
		, AActor* Other
		, UPrimitiveComponent* OtherComp
		, bool bSelfMoved
		, FVector HitLocation
		, FVector HitNormal
		, FVector NormalImpulse
		, const FHitResult& Hit
	) override;

};
