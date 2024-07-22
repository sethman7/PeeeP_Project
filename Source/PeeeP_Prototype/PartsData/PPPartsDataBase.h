// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PPPartsDataBase.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPPartsDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMesh> PartsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> PartsMappingContext;


public:

	virtual void AttachParts(APlayerController* PlayerController);
	virtual void DettachParts(APlayerController* PlayerController);

};
