// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimeLineComponent.h"
#include "PPDoor.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPDoor();
	void OnInteract();

	UFUNCTION()
	void OpenDoor(float value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> Body;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsDoorClosed;
	double Speed;
	double Max;
	double ZLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Timeline",Meta=(AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloat;

	FTimeline Timeline;


};
