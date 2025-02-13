// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PPPoolObject.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Pooling, Warning, All);

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPPoolObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPPPoolObject();

	class AActor* GetPoolObject();
	void ReturnObject(class AActor* InPoolObject);
	bool SetPoolingObjectClass(TSubclassOf<AActor> InPoolingObjectClass);

private:
	TSubclassOf<AActor> PoolingObjectClass;
	TArray<class AActor*> Pool;
};
