// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartsData/PPPartsDataBase.h"
#include "PPGrabPartsData.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGrabPartsData : public UPPPartsDataBase
{
	GENERATED_BODY()
	
public:
	virtual void AttachParts(APlayerController* PlayerController) override;

protected:
	
	TObjectPtr<class UPhysicsHandleComponent> GrabHandle;
};
