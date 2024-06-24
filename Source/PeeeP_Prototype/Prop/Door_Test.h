// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door_Test.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API ADoor_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor_Test();
	void SwitchOnOff();
	void MoveDoor(float value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> Body;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool trigger;
	bool Switch;
	double Z_Loc;
};
