// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/PPAnimInstance.h"
#include "PPGrabAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API UPPGrabAnimInstance : public UPPAnimInstance
{
	GENERATED_BODY()
	
public:
	UPPGrabAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 IsGrab : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	//uint8 IsRelease : 1;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
