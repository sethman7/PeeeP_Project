// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PPPoolableInterface.h"
#include "PPBasket.generated.h"

UCLASS()
class PEEEP_PROTOTYPE_API APPBasket : public AActor, public IPPPoolableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPBasket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	void SetObjectPool(class UPPPoolObject* InObjedtPool) override;
	void ReturnSelf() override;
	void SetActive(bool IsActive) override;

private:

	class UPPPoolObject* ObjectPool;
};
