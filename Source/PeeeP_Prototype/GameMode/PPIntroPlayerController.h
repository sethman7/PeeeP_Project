// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPIntroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PEEEP_PROTOTYPE_API APPIntroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APPIntroPlayerController();

	virtual void BeginPlay() override;

private:

	TSubclassOf<class UUserWidget> Intro;

	TObjectPtr<class UUserWidget> IntroUI;
};
