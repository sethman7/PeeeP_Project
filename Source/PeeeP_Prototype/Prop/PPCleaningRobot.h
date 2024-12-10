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

	UPROPERTY(EditAnywhere)
	float KnockbackStrength;

	UPROPERTY(EditAnywhere)
	float ElectricLossRate;

	UPROPERTY(VisibleAnywhere)
	AActor* GrabbedObject;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaticMeshComponent)
	TObjectPtr<UStaticMeshComponent> Body;

	UFUNCTION()
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	bool IsGrab;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
