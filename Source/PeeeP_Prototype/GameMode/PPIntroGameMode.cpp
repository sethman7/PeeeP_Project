// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PPIntroGameMode.h"

APPIntroGameMode::APPIntroGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/UI/Intro/BP_IntroController.BP_IntroController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
