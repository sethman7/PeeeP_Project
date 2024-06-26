// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
