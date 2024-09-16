// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPSplinePatrolPath.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPSplinePatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPSplinePatrolPath();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	class USplineComponent* Spline;

	UPROPERTY(EditAnyWhere, Category = "SplineController")
	float TotalPathTime;

	UPROPERTY(EditAnyWhere, Category = "SplineController")
	bool bSplineInLoop;

	UPROPERTY(EditDefaultsOnly, Category = "SplineController")
	TSubclassOf<class AActor> ActorToMoveClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	void MoveActor(float Distance);
	float StartTime;
	bool bCanMoveActor;
	bool bIsMovingForward;
	class AActor* ActorToMove;

};
