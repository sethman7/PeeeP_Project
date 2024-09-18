// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPTrap.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	UPROPERTY(EditAnywhere)
	float Rotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
