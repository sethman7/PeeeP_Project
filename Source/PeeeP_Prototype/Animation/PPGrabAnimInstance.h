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
	
protected:
	UPROPERTY()
	TObjectPtr<class APPCharacterPlayer> CharacterPlayer;
	
public:
	UPPGrabAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GrabParts)
	bool bIsGrabbing;
};
